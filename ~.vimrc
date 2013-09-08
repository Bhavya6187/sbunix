"---------------------------------------------------------------------------------------------------------------------------
" 
" VIMRC file
" Author = Dushyant Goyal
"
" Plugins installed - taglist, omnicomplete, alternate, snipmate, cvim, BlockComment, NERD tree, AutomaticTexPlugin.
" conque
"
"---------------------------------------------------------------------------------------------------------------------------

" General_Settings {
  set backspace=2
  set nocp                                "No copatibility with vi
  set expandtab                           "Insert spaces instead of a tab character
  set softtabstop=2                       "Set indentation
  set tabstop=2
  set shiftwidth=2
  set backup                              "Set up location for creating backups of buffers
  set backupdir=$HOME/.vim/vim_backups/
  set directory=$HOME/.vim/vim_swp/
  set incsearch                           "Incremental search
  set hlsearch                            "Highlight text being searched for
  set scrolloff=5                         "Keep buffer between top and bottom of screen
  set wildmode=list,longest               "Emulate terminal for autocomplete
  set hidden                              "Switch buffers without saving
  set backspace=indent,eol,start          "Handle backspaces better
  set mouse=a                             "Use mouse everywhere (even in terminal)
  set mousehide                           "Hide mouse while typing
  set cursorline                          "Show currentline (useful in terminal)
  " GUI Settings {
  if has('gui_running')
    set guioptions-=T                     " remove the toolbar
    set lines=40                          " 40 lines of text instead of 24,
 "   set transparency=5                    " Make the window slightly transparent
  else
    set term=xterm                        " Make arrow and other keys work
  endif
  " }
" }

" Programming {
  filetype plugin on                      "Set plugins
  set undolevels=1000                     "Maximum size of command buffer
  set showmatch                           "Show matching brackets
  set nu                                  "Show line numbers
  if has("syntax")                        "Turn on syntax
  	syntax on
  endif
  set showfulltag                         "Set details about how tags are used
  map <C-\> :tab split<CR>:exec("tag ".expand("<cword>"))<CR> 
  map <A-]> :vsp <CR>:exec("tag ".expand("<cword>"))<CR> 
  map <C-]> :split <CR>:exec("tag ".expand("<cword>"))<CR> 
  map <C-F12> :TlistToggle<CR>
  " C/C++ {
    set cin                               "Indentation for C/C++
    let OmniCpp_NamespaceSearch = 1       "Details for OmniCppComplete
    let OmniCpp_GlobalScopeSearch = 1
    let OmniCpp_ShowAccess = 1
    let OmniCpp_ShowPrototypeInAbbr = 1
    let OmniCpp_MayCompleteDot = 1 
    let OmniCpp_MayCompleteArrow = 1
    let OmniCpp_MayCompleteScope = 1
    let OmniCpp_DefaultNamespaces = ["std", "_GLIBCXX_STD"]
    au CursorMovedI,InsertLeave * if pumvisible() == 0|silent! pclose|endif
    set completeopt=menuone,menu,longest,preview
    set tags+=~/.vim/tags/cpp
  " }
  " LaTeX {
    set grepprg=grep\ -nH\ $*
    filetype indent on
    let g:tex_flavor='latex'
  " }
" }

" My Shortcuts {
  nnoremap ; :
  nnoremap j gj
  nnoremap k gk
  cmap W w                       
  cmap WQ wq
  cmap wQ wq
  cmap Q q
  cmap Tabe tabe
  map <C-N> :NERDTreeToggle<CR>
  map <C-J> <C-W>j<C-W>_
  map <C-K> <C-W>k<C-W>_
  map <C-L> <C-W>l<C-W>_
  map <C-H> <C-W>h<C-W>_
  map <C-K> <C-W>k<C-W>_
" }
