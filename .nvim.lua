local function build_debug() vim.cmd.te('cmake --build build/debug') end

local function run_debug()
	vim.cmd.te('cmake --build build/debug && pushd build/debug/fifed ; ./fifed ; popd')
end

vim.keymap.set('n', '<f4>', build_debug)
vim.keymap.set('n', '<f5>', run_debug)

local function build_release() vim.cmd.te('cmake --build build/release') end

local function run_release()
	vim.cmd.te('cmake --build build/release && pushd build/release/fifed ; ./fifed ; popd')
end

vim.keymap.set('n', '<f16>', build_release)
vim.keymap.set('n', '<f17>', run_release)

vim.cmd [[autocmd BufWritePre *.cpp lua vim.lsp.buf.format()]]
vim.cmd [[autocmd BufWritePre *.hpp lua vim.lsp.buf.format()]]
