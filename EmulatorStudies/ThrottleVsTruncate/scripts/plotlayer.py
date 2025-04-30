#!/usr/bin/env python
import ROOT
import plotting as plot
# import argparse
import math
from array import array
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.TH1.AddDirectory(False)

plot.ModTDRStyle(width=800, height=800, l = 0.07, b = 0.07, r = 0.07, t=0.07)


def IphiRToXY(phi, r, radians_per_module=math.radians(10)):
    return (r * math.cos((phi + 0.5) * radians_per_module), r * math.sin((phi + 0.5) * radians_per_module))


def UVtoXY(layer, u, v, s=1):
    return ((u * s) - (v * s) / 2., v * s * math.cos(math.radians(30.)))


def MakeTrapIphi(iphi, r_min, r_max, radians_per_module=math.radians(10)):
    return MakeTrap(iphi * radians_per_module, (iphi + 1) * radians_per_module, r_min, r_max)


def MakeTrap(phi_min, phi_max, r_min, r_max):
    points_x = [r_min * math.cos(phi_min), r_max * math.cos(phi_min), r_max * math.cos(phi_max), r_min * math.cos(phi_max), r_min * math.cos(phi_min)]
    points_y = [r_min * math.sin(phi_min), r_max * math.sin(phi_min), r_max * math.sin(phi_max), r_min * math.sin(phi_max), r_min * math.sin(phi_min)]
    poly = ROOT.TPolyLine(5, array('d', points_x), array('d', points_y))
    return poly


def MakeHex(pos, s=1):
    x, y = pos
    H = (s * 0.5) / math.cos(math.radians(30.))
    h = math.tan(math.radians(30.)) * (s * 0.5)

    points_x = [x, x + (s * 0.5), x + (s * 0.5), x, x - (s * 0.5), x - (s * 0.5), x]
    points_y = [y + H, y + h, y - h, y - H, y - h, y + h, y + H]

    # for i in xrange(6):
    #     length = math.sqrt(math.pow(points_x[i + 1] - points_x[i], 2.) + math.pow(points_y[i + 1] - points_y[i], 2.))
    #     print '%i, %g' % (i, length)

    hex_poly = ROOT.TPolyLine(7, array('d', points_x), array('d', points_y))
    return hex_poly


def HexCentreLineY(x, y, frac, s=1):
    H = (s * 0.5) / math.cos(math.radians(30.))
    return y + frac * H


def MakeHexUV(layer, UV, s=1):
    return MakeHex(UVtoXY(layer, UV[0], UV[1]), s)


def MakeBorderBoxes(pad):
    boxes = []
    boxes.append(ROOT.TPave(0, 0, pad.GetLeftMargin(), 1, 0, "NBNDC"))
    boxes.append(ROOT.TPave(1. - pad.GetRightMargin(), 0, 1, 1, 0, "NBNDC"))
    boxes.append(ROOT.TPave(0, 1 - pad.GetTopMargin(), 1, 1, 0, "NBNDC"))
    boxes.append(ROOT.TPave(0, 0, 1, pad.GetBottomMargin(), 0, "NBNDC"))
    return boxes


def MakeLayerCanvas(name, xlow, xhigh, ylow, yhigh, palette=None):
    if palette is not None:
        plot.ModTDRStyle(width=856, height=800, l = 0.07, b = 0.07, r = 0.14, t=0.07)
    else:
        plot.ModTDRStyle(width=800, height=800, l = 0.07, b = 0.07, r = 0.07, t=0.07)
    res = {}
    res['canvas'] = ROOT.TCanvas(name, name)
    res['pad'] = plot.OnePad()[0]
    res['frame'] = ROOT.TH2F("frame", "", int(xhigh - xlow), xlow, xhigh, int(yhigh - ylow), ylow, yhigh)
    res['frame'].Draw()
    plot.Set(res['frame'].GetXaxis(), LabelSize=0., TickLength=0.)
    plot.Set(res['frame'].GetYaxis(), LabelSize=0., TickLength=0.)
    res['hexes'] = {}
    for iu in xrange(-15, 30):
        for iv in xrange(-15, 30):
            res['hexes'][(iu, iv)] = MakeHex(UVtoXY(1, iu, iv))
            plot.Set(res['hexes'][(iu, iv)], LineColor=18)
            res['hexes'][(iu, iv)].Draw()
    res['borders'] = MakeBorderBoxes(res['pad'])
    for box in res['borders']:
        box.Draw()
    if palette is not None:
        res['frame'].SetBinContent(0, 0, 1) # hack otherwise COLZ doesn't work
        res['frame'].SetMinimum(palette[0])
        res['frame'].SetMaximum(palette[1])
        res['frame'].SetContour(255)

        res['frame'].Draw('COLZSAME')
        res['pad'].Update()
    plot.FixOverlay()
    return res


