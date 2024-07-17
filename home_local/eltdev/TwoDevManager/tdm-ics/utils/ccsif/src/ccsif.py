#!/usr/bin/env python

"""
@copyright ESO - European Southern Observatory
@brief Shell to interact with CCS IF simulator

Usage: ccsif [OPTIONS]

Options:
  --name TEXT        registered name in Consul
  --uri TEXT         service uri. e.g. zpb.rr://134.171.3.48:22898
  --module TEXT      python module implementing the commands
  --class_name TEXT  Command class
  --timeout INTEGER  timeout for CII MAL requests in ms
  --help             Show this message and exit.

"""

import sys
import click
from pathlib import Path
import subprocess
import ifw.core.stooUtils.consul as consul_utils

@click.command()
@click.option('--name', default='telifsim', help='registered name in Consul')
@click.option('--uri', default='none', help='service uri. e.g. zpb.rr://134.171.3.48:22898')
@click.option('--module', default='tdm.seq.tpl.ccsif_async_commands', help='python module implementing the commands')
@click.option('--class_name', default='CcsifAsyncCommands', help='Command class')
@click.option('--timeout', default=15000, help='timeout for CII MAL requests in ms')
@click.option('--log_level', default='ERROR', help='debugging level',
              type=click.Choice(['ERROR', 'INFO', 'DEBUG'], case_sensitive=False))
def main(name, uri, module, timeout,class_name,log_level):
    try:
        if (uri == "none"):
            cons = consul_utils.ConsulClient()
            uri = cons.get_uri(name)
        else:
            """ removes any quotes an user may add """
            uri = uri.strip(' " " ')
            uri = uri.strip(' \' \' ')
            """ remove / from the uri because it creates problem with MAL """
            if uri.endswith('/'):
                uri = uri[:-1]

    except Exception as e:
        # Handle exceptions or
        print(e)
        raise SystemExit
    print(uri)
    try:
        output = subprocess.run(["acli",
                                 "--module", f"{module}",
                                 "--class_name", f"{class_name}",
                                 "--class_args", f"{uri},{timeout}",
                                 "--history_file", "/.ccsifSh.txt",
                                 "--log_level", f"{log_level}",
                                 "--prompt", "ccsifSh> "])
    except Exception as e:
        print(e)
        raise SystemExit

if __name__ == "__main__":
    main()
