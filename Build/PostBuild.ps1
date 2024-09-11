param(
    [Parameter(Mandatory=$true)]
    [string]
    $solution_path="",
    [Parameter(Mandatory=$true)]
    [string]
    $configuration="",
    [Parameter(Mandatory=$true)]
    [string]
    $project_name="",
    [Parameter(Mandatory=$true)]
    [string]
    $dll_name="")

# $bee_dlls = @(
#     "Bee", 
#     "BeeDebug", 
#     "BeeUtil"
# )

$dlls_path = "$solution_path\\DLL\\$configuration\\"
$project_path = "$solution_path\\Bin\\$configuration\\$project_name\\"

New-Item -Path $project_path -ItemType Directory
Write-Output "Destination is set to $project_path"
# foreach ($i in $bee_dlls)
# {
#     $source = $dlls_path + $i + "\\" + $i + ".dll"
# 
#     Write-Output "Coping item $source"
#     Copy-Item $source $project_path -Force
# }

$source = $dlls_path + $dll_name + "\\" + $dll_name + ".dll"

Write-Output "Coping item $source"
Copy-Item $source $project_path -Force