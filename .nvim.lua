local overseer = require("overseer")

overseer.setup({
	task_list = {
		direction = "bottom", -- Opciones: "left", "right", "bottom"
		min_height = 30,
		-- max_width = { 100, 0.2 },
		-- default_detail = 1,
	},
})

overseer.register_template({
	name = "Run vixel linux",

	params = {
		config_file = {
			type = "string",
			desc = "Config file",
			optional = false,
		},
	},

	builder = function(params)
		return {
			cmd = { "./build/linux/gcc/bin/Debug/vixel" },
			args = { "--config", params.config_file },
			components = {
				"default",
				"on_complete_dispose",
			},
		}
	end,

	priority = 100,
})

overseer.register_template({
	name = "Run vixel windows",

	params = {
		config_file = {
			type = "string",
			desc = "Config file",
			optional = false,
		},
	},
	builder = function(params)
		return {
			cmd = { "./scripts/windows_cmd.sh" },
			args = { "build/windows/bin/Debug/vixel.exe", "--config", params.config_file },
			components = {
				"default",
				"on_complete_dispose",
			},
		}
	end,

	priority = 100,
})

overseer.register_template({
	name = "Windows command",
	params = {
		mi_arg = {
			type = "string",
			desc = "Preset name (e.g., minsizerel)", -- Updated to English
			optional = false,
		},
	},
	builder = function(params)
		return {
			cmd = { "bash" },
			args = { "./scripts/windows_cmd.sh", params.mi_arg },
			cwd = vim.fn.getcwd(),
			components = {
				"default",
				"on_complete_dispose",
			},
		}
	end,
	priority = 180,
})

overseer.register_template({
	name = "Build: Debug",

	builder = function()
		return {
			cmd = { "./scripts/build.sh" },
			args = { "Debug" },
			components = {
				"default",
				"on_complete_dispose",
			},
		}
	end,

	priority = 100,
})

overseer.register_template({
	name = "Build: Release",

	builder = function()
		return {
			cmd = { "./scripts/build.sh" },
			args = { "Release" },
			components = {
				"default",
				"on_complete_dispose",
			},
		}
	end,

	priority = 100,
})
overseer.register_template({
	name = "Build: RelWithDebInfo",

	builder = function()
		return {
			cmd = { "./scripts/build.sh" },
			args = { "RelWithDebInfo" },
			components = {
				"default",
				"on_complete_dispose",
			},
		}
	end,

	priority = 100,
})

overseer.register_template({
	name = "Workflow: Debug",

	builder = function()
		return {
			cmd = { "./scripts/workflow.sh" },
			args = { "Debug" },
			components = {
				"default",
				"on_complete_dispose",
			},
		}
	end,

	priority = 100,
})
overseer.register_template({
	name = "Workflow: Release",

	builder = function()
		return {
			cmd = { "./scripts/workflow.sh" },
			args = { "Release" },
			components = {
				"default",
				"on_complete_dispose",
			},
		}
	end,

	priority = 100,
})
overseer.register_template({
	name = "Workflow: RelWithDebInfo",

	builder = function()
		return {
			cmd = { "./scripts/workflow.sh" },
			args = { "RelWithDebInfo" },
			components = {
				"default",
				"on_complete_dispose",
			},
		}
	end,

	priority = 100,
})

overseer.register_template({
	name = "Workflow: All",

	builder = function()
		return {
			cmd = { "./scripts/full_workflow.sh" },
			components = {
				"default",
				"on_complete_dispose",
			},
		}
	end,

	priority = 100,
})

overseer.register_template({
	name = "Workflow: Custom Argument...",
	params = {
		mi_arg = {
			type = "string",
			desc = "Nombre del preset (ej: minsizerel)",
			optional = false,
		},
	},
	builder = function(params)
		return {
			cmd = { "./scripts/workflow.sh" },
			args = { params.mi_arg },
			components = {
				"default",
				"on_complete_dispose",
			},
		}
	end,

	priority = 180,
})
