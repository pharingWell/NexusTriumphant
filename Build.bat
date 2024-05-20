S:
cd S:\Unreal\Engine
.\Engine\Binaries\Win64>BuildPatchTool.exe
  -OrganizationId="o-wv5chny33efdemtalkwzkfy7ctrydz"
  -ProductId="6d47be9bbcd64778ba6e25e3011ff3e7"
  -ArtifactId=ARTIFACE_GOES_HERE
  -ClientId="xyza7891cyj3aDZSo8PxmNvjuI95XgRb"
  -ClientSecretEnvVar="UNREAL_CLIENT_SECRET"
  -mode=PatchGeneration
  -BuildRoot="<LocationOfLocalBuild>"
  -CloudDir="<YourCloudDir>"
  -BuildVersion="<YourBuildVersion>"
  -AppLaunch="<AppToRun>"
  -AppArgs="<LaunchArguments>"
  -FileAttributeList="<LocationOfAttributesFile>"
  -FileIgnoreList="<LocationOfIgnoreFile>"