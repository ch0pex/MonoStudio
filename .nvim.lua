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
	name = "Run Workflow Script (Local)",

	params = {
		mi_arg = {
			type = "string",
			desc = "workflow.sh <build_type>",
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

	priority = 100,
})
