import argparse
# import tools
import ROOT
# import plotlayer
import plotting
import json
import math
import fileReaders
from array import array

ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.TH1.AddDirectory(False)

plotting.ModTDRStyle(width=800, height=800, l = 0.10, b = 0.10, r = 0.04, t=0.07)

def GetPoints(p_x, p_y, irotate=0):
    ref_arr_x = array('d', p_x)
    ref_arr_y = array('d', p_y)

    rotate = (2 * math.pi) * (irotate / 3.)

    rot_x = []
    rot_y = []

    for i in range(len(ref_arr_x)):
        x = ref_arr_x[i]
        y = ref_arr_y[i]
        rot_x.append(x * math.cos(rotate) - y * math.sin(rotate))
        rot_y.append(x * math.sin(rotate) + y * math.cos(rotate))

    arr_x = array('d', rot_x)
    arr_y = array('d', rot_y)
    return arr_x, arr_y


def ComputePoints(layer, uv, p_x, p_y, shape, points, irotate=0):
    arr_x, arr_y = GetPoints(p_x, p_y, irotate)

    poly = ROOT.TGeoPolygon(len(arr_x) - 1)
    poly.SetXY(arr_x, arr_y)
    poly.FinishPolygon()
    print poly.IsConvex()

    drawPoly = ROOT.TPolyLine(len(arr_x), arr_x, arr_y)

    check = (layer, uv[0], uv[1])

    canv = ROOT.TCanvas('masking_%i_%i_%i' % check, 'masking_%i_%i_%i' % check)
    pad = plotting.OnePad()[0]

    sizer = 60. + ((max(max(arr_x) - min(arr_x), max(arr_y) - min(arr_y))) / 2.)
    mid_x = (max(arr_x) + min(arr_x)) / 2.
    mid_y = (max(arr_y) + min(arr_y)) / 2.

    frame = ROOT.TH2F("frame", "", 50, mid_x - sizer, mid_x + sizer, 50, mid_y - sizer, mid_y + sizer)
    frame.Draw()
    drawPoly.SetLineWidth(2)
    drawPoly.Draw()

    x_in = []
    y_in = []
    x_out = []
    y_out = []

    for point in points:
        inside = poly.Contains(array('d', point))
        if inside:
            x_in.append(point[0])
            y_in.append(point[1])
        else:
            x_out.append(point[0])
            y_out.append(point[1])

    if len(x_in) > 0:
        gr_in = ROOT.TGraph(len(x_in), array('d', x_in), array('d', y_in))
        plotting.Set(gr_in, MarkerColor=1)
        gr_in.Draw('SAMEP')

    if len(x_out) > 0:
        gr_out = ROOT.TGraph(len(x_out), array('d', x_out), array('d', y_out))
        plotting.Set(gr_out, MarkerColor=2)
        gr_out.Draw('SAMEP')
    plotting.DrawTitle(pad, 'Layer=%i, UV=(%i,%i), shape=%s, cells=%i' % (layer, uv[0], uv[1], shape, len(points)), 1)

    canv.Print('.pdf')
    print 'In module %s have %i cells inside and %i cells outside' % (check, len(x_in), len(x_out))


def ToSector(layer, u, v, sector):
    if layer <= 28:
        offset = 0
    elif (layer % 2) == 1:
        offset = -1
    else:
        offset = 1
    if sector == 0:
        return (u, v)
    if sector == 1:
        # u' = v - u
        # v' = -u + offset
        # u = offset - v'
        # v = u' + u
        u_r = offset - v
        v_r = u + u_r
        return (u_r, v_r)
    if sector == 2:
        # u' = -v + offset
        # v' = u - v + offset
        # v = offset - u'
        # u = v' + v - offset
        v_r = offset - u
        u_r = v + v_r - offset
        return (u_r, v_r)
    return None


parser = argparse.ArgumentParser()
parser.add_argument('--legacy', action='store_true', help='Legacy mode, read from hardcoded txt files')
parser.add_argument('--input', '-i', default='hgcal_modmap/geometries/v11.5/geometry.hgcal.txt', help='Input file')
parser.add_argument('--plot-layer', type=int, default=None, help='Make plots for this layer')
parser.add_argument('--cmssw-json', default='data/CMSSW/cmssw_FH.json', help='CMSSW json containing cell positions')
args = parser.parse_args()

modules_inputs = []


if args.legacy:
    with open('data/FE/geomCMSSW1-50_corrected_withmult_F_rotations_subcassettes_v11.1.txt') as poly_file:
        poly_lines = poly_file.readlines()
    with open('data/FE/geomnew_corrected_withmult_F_rotations_v11.1.txt') as mod_file:
        mod_lines = mod_file.readlines()
    for i in range(len(poly_lines)):
        mod = mod_lines[i].split()
        poly = poly_lines[i].split()
        layer = int(mod[0])
        u = int(mod[6])
        v = int(mod[7])
        shape = mod[1]
        if shape in ['F']:
            continue
        p_x = []
        p_y = []
        npoints = int(poly[5])
        for ip in range(npoints + 1):
            p_x.append(float(poly[6 + 2 * ip]))
            p_y.append(float(poly[7 + 2 * ip]))
        modules_inputs.append((layer, u, v, shape, p_x, p_y))
else:
    # origgeo = fileReaders.ReadGeoSourceFile(args.input)
    # for layer in origgeo['layers']:
    #     for key, mod in layer['module_dict'].iteritems():
    #         if mod['shape'] in ['F']:
    #             continue
    #         modules_inputs.append((layer['global_index'], mod["UV"][0], mod["UV"][1], mod["shape"], mod["vx"], mod["vy"]))
    origgeo = fileReaders.Read2021GeoSourceFile(args.input)
    for mb in origgeo:
        mod = mb['modules'][0]
        if mod['shape'] in ['F']:
            continue
        modules_inputs.append((mb['layer'], mod['u'], mod['v'], mod["shape"], mod["vx"], mod["vy"]))


modules_inputs.sort(key=lambda x: tuple((x[0], x[1], x[2])))

if args.plot_layer is not None:
    with open(args.cmssw_json) as jsonfile:
        cmssw_masking = json.load(jsonfile)['masking']

module_pos = {}


for layer, u, v, shape, p_x, p_y in modules_inputs:
    print layer, u, v, shape, p_x, p_y
    for il in [1, -1]:
        for ir in [0, 1, 2]:
            slayer = layer * il
            uv = ToSector(abs(slayer), u, v, ir)
            key = '%s,%s,%s' % (slayer, uv[0], uv[1])
            # Have to mirror for +1
            if il == 1:
                p_x_final = [-1. * x for x in p_x]
            else:
                p_x_final = p_x
            irotate = -1 * il * ir
            if args.plot_layer == layer and key in cmssw_masking:
                ComputePoints(slayer, uv, p_x_final, p_y, shape, cmssw_masking[key], irotate=irotate)
            arr_x, arr_y = GetPoints(p_x_final, p_y, irotate)
            module_pos[key] = {
                "x": list(arr_x),
                "y": list(arr_y)
            }


with open('module_pos.json', 'w') as out_file:
    out_file.write(json.dumps(module_pos, separators=(',', ': '), indent=2))
