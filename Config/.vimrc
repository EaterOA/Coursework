" line numbers
set nu

" indenting based on language
set smartindent

" 4 space indentation
set expandtab
set shiftwidth=4
set tabstop=4

" syntax highlighting
syntax on

" changes syntax highlighting colors
set background=dark

" prevent smartindent from unindenting comments
au! FileType python inoremap # X#

" map :W to :w
command W w

" change tab-complete behavior
""" first tap: complete to longest possible match
""" second+ tap: list autocomplete options
set wildmode=longest,list
