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

$dlls_path = "$solution_path\DLL\$configuration\"
$project_name_arr = @("Project1", "EmptyOpenglProject")

foreach ($i in $project_name_arr) 
{
        $project_path = "$solution_path\Bin\$configuration\$i\"

        New-Item -Path $project_path -ItemType Directory
        Write-Output "Destination is set to $project_path"

        $source = $dlls_path + $dll_name + "\" + $dll_name + ".dll"

        Write-Output "Coping item $source"
        Copy-Item $source $project_path -Force

        if (![System.IO.File]::Exists($project_path + "glew32.dll"))
        {
            Copy-Item "$solution_path\Vendor\glew-2.2.0\bin\Release\x64\glew32.dll" $project_path -Force
        }
}

