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

## What is stored

Commit the `.uproject` file, `Source`, `Config`, `Content`, `.vsconfig`, `.gitignore`, `.gitattributes`, and this README. `Content` includes maps, Blueprints, and external actor and object data; keep these files together.

The project-root `Binaries`, `Intermediate`, `DerivedDataCache`, `Saved`, `.vs`, and generated Visual Studio solutions are ignored and can normally be regenerated. Back up anything you need from `Saved`, such as screenshots or save games, separately. If you add plugins, check their installation instructions: some require prebuilt binaries or other dependencies that must be committed or obtained separately.
