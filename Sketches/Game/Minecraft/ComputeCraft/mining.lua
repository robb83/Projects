require("bot")
require("utils")

-- todo inventory chest
-- todo left, right dig

local bot = createBot()
local w = 25
local h = 50
local flip = false
local roadBlocks = {}
local knowBlocks = {}

hash_set(roadBlocks, "minecraft:cobblestone")
hash_set(roadBlocks, "minecraft:granite")
hash_set(roadBlocks, "minecraft:diorite")
hash_set(roadBlocks, "minecraft:andesite")
hash_set(roadBlocks, "minecraft:stone")
hash_set(roadBlocks, "minecraft:dirt")

hash_set(knowBlocks, "minecraft:granite")
hash_set(knowBlocks, "minecraft:diorite")
hash_set(knowBlocks, "minecraft:andesite")
hash_set(knowBlocks, "minecraft:gravel")
hash_set(knowBlocks, "minecraft:copper_ore")
hash_set(knowBlocks, "minecraft:iron_ore")
hash_set(knowBlocks, "minecraft:cobblestone")
hash_set(knowBlocks, "minecraft:lapis_ore")
hash_set(knowBlocks, "minecraft:dirt")
hash_set(knowBlocks, "minecraft:wall_torch")

local printall = function()
    local detected = {}
    local a, b

    if turtle.detect() then
        a, b = turtle.inspect()
        table.insert(detected, b)
    end

    if turtle.detectDown() then
        a, b = turtle.inspectDown()
        table.insert(detected, b)
    end

    if turtle.detectUp() then
        a, b = turtle.inspectUp()
        table.insert(detected, b)
    end

    for n = 1, #detected do
        if not hash_has(knowBlocks, detected[n].name) then
            local file = fs.open("miner.txt", "a")
            file.write(textutils.serialise(detected[n]))
            file.close()
            hash_set(knowBlocks, detected[n].name)
        end
    end
end

local road = function()
    for n = 1, 16 do
        if turtle.getItemCount(n) > 0 then
            local detail = turtle.getItemDetail(n)
            if hash_has(roadBlocks, detail.name) then
                turtle.select(n)
                turtle.placeDown()
                return true
            end
        end
    end
    return false
end

local fw = function(x)
    for n = 1, x do
        printall()

        road()

        while turtle.detectUp() do
            turtle.digUp()
        end

        while turtle.detect() do
            turtle.dig()
        end

        bot.refuel()
        bot.forward()
        road()

        while turtle.detectUp() do
            turtle.digUp()
        end
    end
end

local bw = function(x)
    for n = 1, x do
        bot.refuel()
        bot.back()
    end
end

flip = h >= 0

if flip then
    bot.turnRight()
    fw(2)
    bw(2)
    bot.turnLeft()
else
    bot.turnLeft()
    fw(2)
    bw(2)
    bot.turnRight()
end

for n = 1, h do

    fw(w)

    if flip then
        bot.turnRight()
        fw(5)
        bw(2)
        bot.turnRight()
    else
        bot.turnLeft()
        fw(5)
        bw(2)
        bot.turnLeft()
    end
    flip = not flip
end
bot.gohome()