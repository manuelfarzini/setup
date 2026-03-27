-- Define file paths
local home = os.getenv("HOME")
local nvim_theme = home .. "/.config/nvim/lua/theme.lua"
local nvim_colorscheme = home .. "/.config/nvim/lua/plugins/colorscheme.lua"
local ghostty_config = home .. "/.config/ghostty/config-theme"
local state = home .. "/Personal/.theme"
local nvim_lualine = home .. "/.config/nvim/lua/plugins/lualine.lua"
local yazi_theme = home .. "/.config/yazi/theme.toml"
local zsh_colors = home .. "/.config/zsh/.zshrc"

-- Read current theme state
local function read_current_theme()
  local file = io.open(state, "r")
  if not file then
    return "dark"
  end
  local theme = file:read("*l")
  file:close()
  return theme
end

-- Write new theme state
local function write_new_theme(theme)
  local file = io.open(state, "w")
  if file then
    file:write(theme)
    file:close()
  end
end

-- Define color palettes (exactly as provided)
local palettes = {
  dark = {
    "000000",
    "1f1f1b",
    "22221d",
    "281f1e",
    "1a230f",
    "dddac8",
    "afa994",
    "44443c",
    "757564",
    "c7723a",
    "c79a3a",
    "5d97d7",
    "c76c6c",
    "cec353",
    "97864e", -- 97864e
    "a7473d",
    "c52b22",
    "e16653",
    "adc04d",
    "788730",
    "97705e",
    "00FFFF",
    "FF00FF",
  },
  light = {
    "ffffff",
    "f7f7f1",
    "f3f3eb",
    "281f1e",
    "1a230f",
    "11110a",
    "606040",
    "ceceb3",
    "a4a475",
    "b9702a",
    "b96e2a",
    "394cae",
    "f74c5a",
    "f74c5d",
    "8a6e13",
    "984138",
    "c52b22",
    "e16653",
    "778f27",
    "667b22",
    "8e6958",
    "00FFFF",
    "FF00FF",
  },
}

-- Function to replace colors in a file by searching substrings directly
local function replace_colors(file_path, old_theme, new_theme)
  local file = io.open(file_path, "r")
  if not file then
    return
  end
  local content = file:read("*all")
  file:close()

  -- Replace each color by searching the exact substring
  for i, old_color in ipairs(palettes[old_theme]) do
    local new_color = palettes[new_theme][i]
    content = content:gsub(old_color, new_color)
  end

  file = io.open(file_path, "w")
  if file then
    file:write(content)
    file:close()
  end
end

-- Main execution
local current_theme = read_current_theme()
local new_theme = (current_theme == "dark") and "light" or "dark"

write_new_theme(new_theme)
replace_colors(nvim_theme, current_theme, new_theme)
replace_colors(nvim_colorscheme, current_theme, new_theme)
replace_colors(ghostty_config, current_theme, new_theme)
replace_colors(nvim_lualine, current_theme, new_theme)
replace_colors(yazi_theme, current_theme, new_theme)
replace_colors(zsh_colors, current_theme, new_theme)

print("Theme switched to: " .. new_theme)

-- Color monitoring section for Neovim highlighting plugin

--# Dark Theme Colors
--# bblack  : #000000
--# black1  : #1f1f1b
--# line    : #22221d
--# blackR  : #281f1e
--# blackG  : #1a230f
--# white1  : #dddac8
--# grey1   : #afa994
--# grey2   : #44443c
--# grey3   : #757564
--# orange1 : #c7723a
--# orange2 : #c79a3a
--# blue1   : #5d97d7
--# err     : #c76c6c
--# warn    : #cec353
--# dark1   : #97864e
--# red1    : #a7473d
--# red2    : #c52b22
--# red3    : #e16653
--# green1  : #adc04d
--# green2  : #788730
--# comment : #97705e
--# test2   : #00FFFF
--# test1   : #FF00FF

-- grey2_old   : #c2b58b
-- grey3_old   : #ac9863
-- Light Theme Colors
-- bblack  : #ffffff
-- black1  : #f7f7f1
-- line    : #f3f3eb
-- blackR  : #281f1e
-- blackG  : #1a230f
-- white1  : #11110a
-- grey1   : #606040
-- grey2   : #ceceb3
-- grey3   : #a4a475
-- orange1 : #b9702a
-- orange2 : #b96e2a
-- blue1   : #394cae
-- err     : #f74c5a
-- warn    : #f74c5d  
-- dark1   : #8a6e13
--#c76c6c
--#916A22
--#f7774c
--#c79a6c
--#347dcd
-- red1    : #984138
-- red2    : #984338
-- red3    : #e16653
-- green1  : #778f27
-- green2  : #667b22
-- comment : #8e6958
-- test2   : #00FFFF
-- test1   : #FF00FF
