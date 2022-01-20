--
--  Helper script allowing difference codegen scripts to be run from a single script (this one)
--

local cli = require("jclib-cli")
local _args, _ops = cli.parse(...)


print("args = ")
for i,v in ipairs(_args) do
    print("\t" .. v)
end
print("ops = ")
for k,v in pairs(_ops) do
    print("\t" .. k .. " = " .. v)
end
