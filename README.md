# CLAUDE.md — Proland Core & Terrain → UE5 Porting Guide

## Project Overview

Port Proland's **Core** and **Terrain** modules from OpenGL/C++ to Unreal Engine 5.7.
Reference source: `Proland_Reference/` folder.
Target: GIS planetary terrain rendering using Nanite + UE5 task system.

---

## Naming Conventions

| Rule | Example |
|------|---------|
| Ported Proland classes use `F` prefix | `FTileCache`, `FTerrainNode` |
| UE asset/config classes use `U` prefix | `UTileStorageConfig` |
| Actor classes use `A` prefix | `APlanetForgeActor` |
| No `U` prefix on ported logic classes | ✗ `UTileProducer` → ✓ `FTileProducer` |

---

## Directory Structure

```
Source/
├── Core/
│   ├── math/               # noise
│   ├── producer/           # TileStorage TileLayer TileProducer TileCache
│   │                         GPUTileStorage CPUTileStorage
│   └── terrain/            # CylindricalDeformation Deformation ReadbackManager
│                             SphericalDeformation TerrainNode TerrainQuad
│                             TileSampler TileSamplerZ
├── Terrain/
│   ├── dem/                # CPUElevationProducer ElevationProducer NormalProducer
│   │                         ResidualProducer
│   └── ortho/              # OrthoCPUProducer OrthoGPUProducer OrthoProducer
│                             TextureLayer
└── PlanetForgeActor.h/.cpp
```

Each listed name without `/` suffix = one `.h` + one `.cpp` pair.

---

## Architecture Mapping

### 1. Task System → UE::Tasks

Replace Proland's scheduler with `UE::Tasks`:

```cpp
// Proland pattern → UE5 pattern
UE::Tasks::Launch(UE_SOURCE_LOCATION, [this]()
{
    // tile work here
}, UE::Tasks::ETaskPriority::BackgroundNormal);

// Task with prerequisite
UE::Tasks::FTask UpdateTask = UE::Tasks::Launch(..., [](){}, Prerequisites);
```

Three main update loops (implement as recurring tasks or tick-driven):
- `FUpdateTileSamplersTask` — per-frame sampler state update
- `FUpdateTerrainTask` — quad-tree LOD update
- `FDrawTerrainTask` — mesh/draw call submission

### 2. View / Camera → FViewContext

Replace all direct camera/viewport queries with a single struct:

```cpp
// Core/terrain/ViewContext.h
struct FViewContext
{
    FVector      CameraLocation;
    FRotator     CameraRotation;
    FMatrix      ViewMatrix;
    FMatrix      ProjectionMatrix;
    float        FOV;           // degrees
    float        AspectRatio;
    float        NearClip;
    float        FarClip;
    FIntPoint    ViewportSize;
};
```

Populate via `FViewContextUtil` (see §6). Pass `const FViewContext&` everywhere instead of querying GEngine/viewport directly.

### 3. Config → UDataAsset

One `UDataAsset` subclass per logical config group:

```cpp
// UTileStorageConfig : public UDataAsset
UPROPERTY(EditAnywhere) int32 TileSize   = 256;
UPROPERTY(EditAnywhere) int32 Capacity   = 512;
UPROPERTY(EditAnywhere) FString FilePath;

// UTerrainConfig : public UDataAsset
UPROPERTY(EditAnywhere) int32 MaxLevel   = 16;
UPROPERTY(EditAnywhere) float SplitFactor = 2.f;
// ... etc.
```

Reference assets in `APlanetForgeActor` as `UPROPERTY(EditAnywhere)` pointers.

### 4. Rendering → UStaticMesh + Nanite

- Build `FStaticMeshRenderData` on worker threads (allowed by UE5).
- Register/finalize on game thread via `UStaticMesh::PostLoad()` or `InitResources()`.
- Enable Nanite per mesh: `StaticMesh->NaniteSettings.bEnabled = true;`
- Use `UStaticMeshComponent` or procedural mesh updated each LOD change.

Thread-safe render data pattern:
```cpp
// Worker thread
TUniquePtr<FStaticMeshRenderData> RenderData = MakeUnique<FStaticMeshRenderData>();
// fill LOD sections, vertex buffers, index buffers...

// Game thread
ENQUEUE_RENDER_COMMAND(UploadTileMesh)([RenderData = MoveTemp(RenderData), Mesh](FRHICommandListImmediate& RHICmdList)
{
    Mesh->SetRenderData(MoveTemp(RenderData));
});
```

---

## Core Module Implementation Notes

### TileCache / TileStorage
- `FTileCache`: LRU eviction, thread-safe with `FCriticalSection` or `FRWLock`.
- `FGPUTileStorage`: wraps `FTexture2DRHIRef` array (texture atlas per level).
- `FCPUTileStorage`: heap-allocated byte buffers, pooled with `TArray<TArray<uint8>>`.

### TerrainNode / TerrainQuad
- Quadtree split/merge driven by `FViewContext` (distance + screen-space error).
- Keep split logic on game thread; tile *production* on worker threads.

### TileSampler / TileSamplerZ
- `FTileSampler`: maps a `FTileProducer` output to a shader parameter slot.
- `FTileSamplerZ`: elevation-specific variant; feeds height into `FUpdateTerrainTask`.

### Deformation (Spherical / Cylindrical)
- Convert Proland's `mat4d` → `FMatrix` (column-major, same layout).
- `FSphericalDeformation`: planet-radius scaling; store radius in `UTerrainConfig`.

---

## Terrain Module Implementation Notes

### DEM producers
| Class | Role |
|---|---|
| `FCPUElevationProducer` | Reads raw DEM files on CPU, outputs `FCPUTileStorage` tiles |
| `FElevationProducer` | GPU upscale/blend of elevation tiles → `FGPUTileStorage` |
| `FNormalProducer` | Derives normals from elevation GPU tiles |
| `FResidualProducer` | Loads residual DEM files, CPU side |

### Ortho producers
| Class | Role |
|---|---|
| `FOrthoCPUProducer` | Reads imagery tiles from disk |
| `FOrthoGPUProducer` | GPU blend/mip of imagery tiles |
| `FOrthoProducer` | Orchestrates CPU→GPU upload |
| `FTextureLayer` | Shader-facing wrapper for an ortho tile atlas |

---

## FViewContextUtil

Utility class (no UObject, pure static or singleton) supporting three execution contexts:

```cpp
// Core/terrain/ViewContextUtil.h
class MYPROJECT_API FViewContextUtil
{
public:
    // Call from APlanetForgeActor::Tick or BeginPlay
    static FViewContext BuildFromEditorViewport();   // Editor mode
    static FViewContext BuildFromPIEViewport();       // PIE
    static FViewContext BuildFromGamePlayer(APlayerController* PC); // Game

    // Auto-selects correct source
    static FViewContext Build(UWorld* World);
};
```

Implementation hints:
- **Editor**: `GEditor->GetActiveViewport()` → `FEditorViewportClient`.
- **PIE / Game**: `World->GetFirstPlayerController()` → `APlayerController::GetPlayerViewPoint()`.
- Cache `FViewContext` each frame on game thread; never query viewport on worker threads.

---

## APlanetForgeActor

```cpp
UCLASS()
class MYPROJECT_API APlanetForgeActor : public AActor
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, Category="Config") UTileStorageConfig* TileStorageConfig;
    UPROPERTY(EditAnywhere, Category="Config") UTerrainConfig*     TerrainConfig;

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent&) override;
    void OnEditorTick(float DeltaSeconds); // registered via FEditorDelegates
#endif

private:
    FViewContext         CurrentView;
    TUniquePtr<FTerrainNode> RootNode;

    UE::Tasks::FTask     UpdateSamplersTask;
    UE::Tasks::FTask     UpdateTerrainTask;
    UE::Tasks::FTask     DrawTerrainTask;

    void ScheduleFrameTasks();
};
```

- Register an editor tick via `FEditorDelegates::PostPIEStarted` / `FTicker` for Editor mode.
- In `Tick`: call `FViewContextUtil::Build(GetWorld())`, store in `CurrentView`, then `ScheduleFrameTasks()`.

---

## General Coding Rules

1. **No raw OpenGL** — replace all GL calls with RHI (`FRHICommandList`, `FTexture2DRHIRef`, etc.).
2. **Double precision math** — Proland uses `double`; keep `double` for world-space positions, use `FVector` (float) only for render-local coords. Consider `FLargeWorldCoordinates`.
3. **Thread safety** — tile production runs on worker threads; RHI uploads via `ENQUEUE_RENDER_COMMAND`; game-thread data via `AsyncTask(ENamedThreads::GameThread, ...)`.
4. **No blocking waits on game thread** — use `UE::Tasks::FTask::IsCompleted()` checks or pipe results through callbacks.
5. **DataAsset references** — always null-check config asset pointers before use.
6. **Include order** — UE headers first, then project headers, then `Proland_Reference` types (if needed during porting).

---

## Token-Saving Conventions for Claude Code Sessions

- Reference `Proland_Reference/<file>` by relative path; do not paste full Proland source.
- When asking Claude to implement a class, specify: **class name, input types, output types, threading context** — omit unrelated classes.
- Implement one producer or one subsystem per session to stay within context.
- Prefix prompts with the target file path so Claude writes directly to the correct `.h`/`.cpp`.

---

## Build Module Setup (MyProject.Build.cs)

```csharp
PublicDependencyModuleNames.AddRange(new string[]
{
    "Core", "CoreUObject", "Engine", "RenderCore", "RHI",
    "StaticMeshDescription", "MeshDescription",
    "Tasks"          // UE::Tasks
});
PrivateDependencyModuleNames.AddRange(new string[]
{
    "Renderer", "RenderCore"
});
// Editor-only
if (Target.bBuildEditor)
{
    PrivateDependencyModuleNames.Add("UnrealEd");
}
```
