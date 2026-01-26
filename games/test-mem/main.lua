local dt_count = 0
local startc, endc = 0, 0

function smgf.init()
  startc = collectgarbage("count")
end

function smgf.update(dt)
  dt_count = dt_count + dt

  if dt_count > 1 then
    dt_count = dt_count - 1

    endc = collectgarbage("count")

    local diff = endc - startc

    print(string.format("delta: %.3f KB/s, total: %.1f KB", diff, endc))
    startc = endc
  end
end
