# UnrealPlayground

An Unreal Engine C++ project. The project file currently specifies Unreal Engine **5.8** as its engine association.

## Prerequisites

- A compatible Unreal Engine installation (see `UnrealPlayground.uproject` for the engine association).
- Visual Studio with the **Game development with C++** workload and the Windows SDK required by your Unreal Engine version. The included `.vsconfig` can help install the project's Visual Studio components.
- Git and Git LFS. Check GitHub's current Git LFS storage and bandwidth limits before uploading large assets.

## Clone and rebuild on Windows

Run these commands in PowerShell, from the directory where you want the project folder to be created:

```powershell
git lfs install
git clone https://github.com/arifpurwandaru/UnrealPlayground.git
cd UnrealPlayground
git lfs pull
```

Install the engine version specified in `UnrealPlayground.uproject`. If needed, right-click the `.uproject` file and select **Switch Unreal Engine version** to associate it with the installed engine. Changing versions can require migration; back up the project first.

Right-click `UnrealPlayground.uproject` and select **Generate Visual Studio project files**. Open the generated solution in Visual Studio, select **Development Editor** and **Win64**, and build the project. Then open `UnrealPlayground.uproject` in Unreal Editor. The first build and editor launch may take time while generated files and shaders are rebuilt.

## First-time repository setup

If the project has not yet been added to Git, run the following in PowerShell from the project root. Install Git LFS first, and configure tracking **before** adding assets:

```powershell
git init
git lfs install
git lfs track "*.uasset" "*.umap"
git add .
git status
```

Review the staged files for secrets, private data, and content you cannot share. Ensure `.gitattributes` is staged and Unreal assets are tracked by LFS. Then, for an empty GitHub repository:

```powershell
git commit -m "Initial Unreal project"
git branch -M main
git remote add origin https://github.com/arifpurwandaru/UnrealPlayground.git
git push -u origin main
```

If the remote already contains commits, reconcile its history before pushing instead of overwriting it. To verify assets are managed by LFS, run `git lfs ls-files` after staging or committing them.

## Srigala controls and animation

For a Blueprint derived from `ASrigala`, assign `SK_Wolf` to the inherited **Mesh**, align its paws with the capsule bottom, and configure the existing movement/look input actions and mapping context. Remove any old Floating Pawn Movement or duplicate wolf mesh component. Character Movement handles gravity and ground collision; the floor must block Pawn.

- Movement follows camera yaw only. Mouse pitch does not make the wolf fly.
- Default movement walks at 180 cm/s; hold **Left Shift** to run at 450 cm/s. These speeds are editable in the Blueprint's Movement defaults; Run Speed must exceed Walk Speed. Run Action can use the native default or a Blueprint-assigned Boolean input action with no Hold/Tap triggers for immediate hold-to-run. Shift is mapped to the final selected action during input setup, and releasing or canceling that same action restores walking; no separate Stop Run Action is needed.
- The follow camera attaches to the spring-arm tip and orbits using controller rotation. At BeginPlay the rig resets stored component transforms, offsets, rotation inheritance, and lag settings; use **Camera Distance** (default 250 cm) rather than moving the Camera component to adjust distance. **Look Sensitivity** defaults to 1; try 0.5 for slower mouse movement. These settings are under Camera on the Blueprint or placed actor. The player must possess Srigala and use it as the view target; avoid another pawn auto-possessing Player 0 or Blueprint camera logic that selects a different view target.
- **Use Simple Locomotion Animation** defaults to enabled. It plays the existing in-place wolf idle, walk, and run clips according to actual horizontal speed, without restarting them every frame. Releasing movement lets Character Movement decelerate and returns to idle below 3 cm/s. There is no separate stop clip in this wolf pack.
- This is simple clip switching, not a blended Animation Blueprint or foot IK. Disable **Use Simple Locomotion Animation** before assigning a custom Animation Blueprint. The three animation assets are editable under Animation and must match the inherited mesh's skeleton; invalid assignments log an error.

After native header or component changes, close Unreal Editor, rebuild the Editor target in Visual Studio, and reopen the project. An active Live Coding session can block the normal Editor build.

## What is stored

Commit the `.uproject` file, `Source`, `Config`, `Content`, `.vsconfig`, `.gitignore`, `.gitattributes`, and this README. `Content` includes maps, Blueprints, and external actor and object data; keep these files together.

The project-root `Binaries`, `Intermediate`, `DerivedDataCache`, `Saved`, `.vs`, and generated Visual Studio solutions are ignored and can normally be regenerated. Back up anything you need from `Saved`, such as screenshots or save games, separately. If you add plugins, check their installation instructions: some require prebuilt binaries or other dependencies that must be committed or obtained separately.
