local home = os.getenv("HOME")

local introot = pathJoin(home, "INTROOT")
setenv("INTROOT", introot)
setenv("PREFIX", introot)

local dataroot = pathJoin(home, "DATAROOT")
setenv("DATAROOT", dataroot)

load("introot")

prepend_path("CFGPATH", "/home_local/eltdev/TwoDevManager/tdm-resource")