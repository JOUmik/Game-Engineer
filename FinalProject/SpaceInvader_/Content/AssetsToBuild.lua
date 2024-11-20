--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	shaders =
	{
		{ path = "Shaders/Vertex/standard.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/standard.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/Green.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/Red.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/Oriange.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/Gray.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/BulletShader.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/PlayerShader.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/Dark.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/animatedColor.shader", arguments = { "fragment" } },
		{ path = "Shaders/Vertex/vertexInputLayout_mesh.shader", arguments = { "vertex" } },
	},
	meshes = {
		{ path = "Meshes/cube.lua" },
		{ path = "Meshes/plain.lua" },
		{ path = "Meshes/Ground.lua" },
		{ path = "Meshes/Sphere.lua" },
		{ path = "Meshes/LaserBullet.lua" },
		{ path = "Meshes/Enemy.lua" },
		{ path = "Meshes/Player.lua" },
		{ path = "Meshes/Block.lua" },
		{ path = "Meshes/GameOverBlock.lua" },
	},
	audio = {
		{ path = "Audio/Laser.mp3"},
		{ path = "Audio/Explosion.mp3"},
		{ path = "Audio/Win.mp3"},
		{ path = "Audio/Fail.mp3"},
		{ path = "Audio/Background.mp3"},
	}
}
