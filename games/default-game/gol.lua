---@class GameOfLife
---@field size number Size in pixels of each cell in the grid
---@field width number Width (in cells) of the board
---@field height number Height (in cells) of the board
---@field cells number[] The grid
---@field nb_steps number The number of steps
local GameOfLife = {}

---Creates a new instance
---@return GameOfLife
function GameOfLife:create()
  local obj = {}
  setmetatable(obj, {__index = GameOfLife})
  return obj
end

---Creates a GameOfLife
---@param width number Width (in pixels) of the board
---@param height number Height (in pixels) of the board
function GameOfLife:init(width, height)
  self.size = 8
  self.width = width / self.size
  self.height = height / self.size
  self.cells = {}
  self.nb_steps = 0

  self:randomize_cells()
  return self
end

---Draws the grid
function GameOfLife:draw()
  for i = 0, self.width * self.height do
    local y = math.floor(i / self.width)
    local x = i % self.width

    if self.cells[i] == 1 then
      smgf.graphics.set_color(0x88, 0x7e, 0xcb)
    else
      smgf.graphics.set_color(0x50, 0x45, 0x9b)
    end

    smgf.graphics.draw_rectfill(x * self.size, y * self.size, self.size,
        self.size)
  end
end

---Randomises every cell in the grid
function GameOfLife:randomize_cells()
  for i = 0, self.width * self.height do
    self.cells[i] = math.random(0, 1)
  end
end

---Clear every cell in the grid
function GameOfLife:clear_cells()
  for i = 0, self.width * self.height do
    self.cells[i] = 0
  end
end

---Execute one step of the simulation
function GameOfLife:step()
  self.nb_steps = self.nb_steps + 1
  local new_cells = {}
  for y = 0, self.height - 1 do
    for x = 0, self.width - 1 do
      -- we copy the old value of the cell to the new table
      local cur_pos = y * self.width + x
      new_cells[cur_pos] = self.cells[cur_pos]

      -- we take the adjacent cells:
      -- north-west, north, north-east, west,
      -- east, west-south, south, east-south
      local nw, n, ne, w, e, ws, s, se = 0, 0, 0, 0, 0, 0, 0, 0

      if x > 0 and y > 0 then
        nw = self.cells[cur_pos - self.width - 1]
      end
      if y > 0 then
        n = self.cells[cur_pos - self.width]
      end
      if x < self.width - 1 and y > 0 then
        ne = self.cells[cur_pos - self.width + 1]
      end
      if x > 0 then
        w = self.cells[cur_pos - 1]
      end
      if x < self.width - 1 then
        e = self.cells[cur_pos + 1]
      end
      if x > 0 and y < self.height - 1 then
        ws = self.cells[cur_pos + self.width - 1]
      end
      if y < self.height - 1 then
        s = self.cells[cur_pos + self.width]
      end
      if x < self.width - 1 and y < self.height - 1 then
        se = self.cells[cur_pos + self.width + 1]
      end

      local nb_alive_cells = nw + n + ne + w + e + ws + s + se

      -- if needed, we change the cell's state (Conway's conditions)
      if self.cells[cur_pos] == 0 then
        -- if the current cell is dead
        if nb_alive_cells == 3 then
          -- reproduction, the cell become alive
          new_cells[cur_pos] = 1
        end
      else -- if the current cell is alive
        if nb_alive_cells < 2 or nb_alive_cells > 3 then
          -- under-population OR overcrowding, the cell dies
          new_cells[cur_pos] = 0
        end
      end
    end
  end
  self.cells = new_cells
end

return GameOfLife
