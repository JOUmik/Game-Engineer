--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	shaders =
	{
		{ path = "Shaders/Vertex/standard.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/standard.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/animatedColor.shader", arguments = { "fragment" } },
		{ path = "Shaders/Vertex/vertexInputLayout_mesh.shader", arguments = { "vertex" } },
	},
	meshes = {
		{ path = "Meshes/house.lua" },
		{ path = "Meshes/cube.lua" },
		{ path = "Meshes/chimney.lua" },
		{ path = "Meshes/plain.lua" },
		{ path = "Meshes/Ground.lua" },
		{ path = "Meshes/Tree.lua" },
		{ path = "Meshes/Donut.lua" },
		{ path = "Meshes/Donut1.lua" },
	}
}
