local overseer = require("overseer")

overseer.setup({
	task_list = {
		direction = "bottom", -- Opciones: "left", "right", "bottom"
		min_height = 50,
		-- max_width = { 100, 0.2 },
		-- default_detail = 1,
	},
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
