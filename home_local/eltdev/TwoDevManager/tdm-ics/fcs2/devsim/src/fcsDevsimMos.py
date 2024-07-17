#!/usr/bin/env python3

"""
@copyright ESO - European Southern Observatory
@brief Device Simulator for the standard Mos Device Manager - deployment.
"""

import sys
import logging
import asyncio

import ifw.core.opcualib.srvlib.serverbase as opcua_sm_srv
import tdm.fcs.devsim.mos.mos

async def main():
    sim_srv = tdm.fcs.devsim.mos.mos.DevsimMos()
    action_mgr = tdm.fcs.devsim.mos.mos.ActionMgr(sim_srv)
    data_object = opcua_sm_srv.Data()
    await sim_srv.execute(sys.argv, action_mgr, data_object)

if __name__ == "__main__":
    asyncio.run(main())

# EOF
