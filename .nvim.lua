-- DEBUG
vim.api.nvim_set_keymap('n', '<f4>', ':te cmake --build build/debug<cr>', { noremap = true, silent = true })
vim.api.nvim_set_keymap('n', '<f28>', ':te cmake --build build/debug --clean-first<cr>',
	{ noremap = true, silent = true })
vim.api.nvim_set_keymap('n', '<f5>', ':te build/debug/fifed/fifed<cr>', { noremap = true, silent = true })

-- RELEASE
vim.api.nvim_set_keymap('n', '<f16>', ':te cmake --build build/release<cr>', { noremap = true, silent = true })
vim.api.nvim_set_keymap('n', '<f29>', ':te cmake --build build/release --clean-first<cr>',
	{ noremap = true, silent = true })
vim.api.nvim_set_keymap('n', '<f17>', ':te build/release/fifed/fifed<cr>', { noremap = true, silent = true })
