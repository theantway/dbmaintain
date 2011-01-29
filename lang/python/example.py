import sys
sys.path.append('.')

from dbmaintain import DbMaintain
dbm=DbMaintain()
dbm.setConfigFile('../../examples/python-runner/test.config')
dbm.update()
