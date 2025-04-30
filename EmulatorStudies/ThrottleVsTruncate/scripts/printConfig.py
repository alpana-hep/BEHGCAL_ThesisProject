import ROOT
import json
import sys

if len(sys.argv) >= 2:
    fin = ROOT.TFile(sys.argv[1])
    jsonstring = fin.Get('config').GetString().Data()
    print(jsonstring)
    js = json.loads(fin.Get('config').GetString().Data())
    # print(js)

    if len(sys.argv) >= 3:
        jsout = sys.argv[2]
        with open(jsout, 'w') as outfile:
            outfile.write(json.dumps(js, indent=2))
