import argparse
import tools
import ROOT
import plotlayer
import plotting


parser = argparse.ArgumentParser()
parser.add_argument('--rate-summary-file', default='rate_summary_file_EOL_fixdensity.txt')
parser.add_argument('--mapping-file', default='data/FE/FeMappingV8.txt')
parser.add_argument('--layout-file', default='data/FE/MbLayoutV1.json')
# parser.add_argument('--plots', action='store_true')
args = parser.parse_args()


mapping = tools.BuildMapping(50, args.mapping_file)

data = tools.ReadRateSummaryFile(args.rate_summary_file)

mbs = tools.ReadMBJson(args.layout_file)

mb_cols = [40, 41, 42, 45, 46, 47, 48, 30, 33, 34, 40, 41, 42, 45, 46, 47, 48, 30, 33, 34, 40, 41, 42, 45, 46, 47, 48, 30, 33, 34, 40, 41, 42, 45, 46, 47, 48, 30, 33, 34]
issue_labels = ['LD-F>2', 'LD-P>1', 'LD-Wagon>3', 'LD-engine>6', 'HD-engine>14']
issue_cols = [1, 2, 4, 8, 6]

issue_h = []

for idx in xrange(len(issue_labels)):
    issue_h.append(ROOT.TH1F('x', '', 1, 0, 1))
    issue_h[-1].SetLineColor(issue_cols[idx])
    issue_h[-1].SetLineWidth(3)
    issue_h[-1].SetTitle(issue_labels[idx])

issues = [([0] * 50) for X in range(len(issue_labels))]
# print issues
elink_label = 'elinks'
for il in xrange(50):
    ilG = il + 1

    plot = plotlayer.MakeLayerCanvas('elink_constraints_%.2i' % ilG, -6, 12, -1, 17)
    plot['cmssw'] = {}

    latex = ROOT.TLatex()
    latex.SetTextFont(42)
    latex.SetTextSize(0.01)
    latex.SetTextAlign(22)

    latexError = ROOT.TLatex()
    plotting.Set(latexError, TextFont=62, TextSize=0.05, TextAlign=22)
    issue_xy = [list() for X in range(len(issue_labels))]
    mod_dict = mapping['layers'][il]['module_dict']

    for UV, module in mod_dict.iteritems():
        if module['type'] >= 2:
            continue
        if module['type'] <= 1:
            plot['cmssw'][UV] = plotlayer.MakeHexUV(ilG, UV)
        else:
            plot['cmssw'][UV] = plotlayer.MakeTrapIphi(UV[1], 10.5, 12)

    for key, val in mbs[il].iteritems():
        modtype = mod_dict[val['I'][0]]['type']
        print '>> Checking layer %i, MB type %i with I: %s, O: %s' % (ilG, modtype, val['I'], val['O'])
        for UV in val['I'] + val['O']:
            if UV not in data[il] or mod_dict[UV]['shape'] in ['c']:
                continue
            mapping_nelinks = mod_dict[UV]['lpGBT_id1_elinks'] + (mod_dict[UV]['lpGBT_id2_elinks'] * (mod_dict[UV]['lpGBT_id2_elinks'] > 0))
            if data[il][UV]['ref_elinks'] != mapping_nelinks:
                print '>> ELINK MISMATCH: SectorEcondAsicTest=%i MappingFile=%i,%i' % (data[il][UV]['ref_elinks'], mod_dict[UV]['lpGBT_id1_elinks'], mod_dict[UV]['lpGBT_id2_elinks'])
        if modtype == 1:
            for UV in val['I'] + val['O']:
                x, y = plotlayer.UVtoXY(ilG, UV[0], UV[1])
                if UV not in data[il] or mod_dict[UV]['shape'] in ['c']:
                    # print '>> SKIPPING %s' % (UV,)
                    continue
                if mod_dict[UV]['shape'] in ['F', 'L', 'R', 'b']:
                    if data[il][UV][elink_label] > 2:
                        print '>> VIOLATION [%s] in %s' % (issue_labels[0], UV)
                        issues[0][il] += 1
                        issue_xy[0].append((x, y))
                else:
                    if data[il][UV][elink_label] > 1:
                        print '>> VIOLATION [%s] in %s' % (issue_labels[1], UV)
                        issues[1][il] += 1
                        issue_xy[1].append((x, y))
                        # plotting.Set(latexError, TextColor=issue_cols[1])
                        # latexError.DrawLatex(x, y, 'X')

        n_wagon_i = 0
        n_wagon_o = 0
        for UV in val['I']:
            if UV not in data[il] or mod_dict[UV]['shape'] in ['c']:
                continue
            n_wagon_i += data[il][UV][elink_label]
        for UV in val['O']:
            if UV not in data[il] or mod_dict[UV]['shape'] in ['c']:
                continue
            n_wagon_o += data[il][UV][elink_label]
        if modtype == 1:
            if n_wagon_i > 3:
                print '>> VIOLATION [%s] in %s' % (issue_labels[2], val['I'])
                issues[2][il] += 1
                for UV in val['I']:
                    issue_xy[2].append(plotlayer.UVtoXY(ilG, UV[0], UV[1]))
            if n_wagon_o > 3:
                print '>> VIOLATION [%s] in %s' % (issue_labels[2], val['O'])
                issues[2][il] += 1
                for UV in val['O']:
                    issue_xy[2].append(plotlayer.UVtoXY(ilG, UV[0], UV[1]))
            if n_wagon_i + n_wagon_o > 6:
                print '>> VIOLATION [%s] in %s' % (issue_labels[3], (val['I'] + val['O']))
                issues[3][il] += 1
                for UV in val['I'] + val['O']:
                    issue_xy[3].append(plotlayer.UVtoXY(ilG, UV[0], UV[1]))

                issue_xy[3].append((x, y))
        if modtype == 0:
            if n_wagon_i + n_wagon_o > 14:
                print '>> VIOLATION [%s] in %s' % (issue_labels[4], (val['I'] + val['O']))
                issues[4][il] += 1
                for UV in val['I'] + val['O']:
                    issue_xy[4].append(plotlayer.UVtoXY(ilG, UV[0], UV[1]))



        for UV in val['I']:
            plot['cmssw'][UV].SetFillColor(mb_cols[key])
        for UV in val['O']:
            plot['cmssw'][UV].SetFillColor(mb_cols[key])

    for UV, module in mod_dict.iteritems():
        if module['type'] >= 2:
            continue

        plot['cmssw'][UV].Draw('F')
        plot['cmssw'][UV].Draw()
        if module['type'] <= 1:
            x, y = plotlayer.UVtoXY(ilG, UV[0], UV[1])
            latex.DrawLatex(x, y, '%i, %i' % UV)
            latex.DrawLatex(x, plotlayer.HexCentreLineY(x, y, -0.75), module['shape'])

    legend = ROOT.TLegend(0.12, 0.8, 0.9, 0.90, '', 'NBNDC')
    legend.SetNColumns(len(issue_labels))
    for i in xrange(len(issue_labels)):
        legend.AddEntry(issue_h[i], '', 'L')
        plotting.Set(latexError, TextColor=issue_cols[i])
        if i <= 1:
            plotting.Set(latexError, TextSize=0.05)
        elif i <= 2:
            plotting.Set(latexError, TextSize=0.04)
        else:
            plotting.Set(latexError, TextSize=0.025)
        for x, y in issue_xy[i]:
            latexError.DrawLatex(x, y, 'X')
    legend.Draw()

    plotting.DrawTitle(plot['pad'], 'Layer %i' % ilG, 1)
    plot['canvas'].Print('.pdf')
    plot['canvas'].Print('.png')
nissues = len(issue_labels)
# print issues
print '%-15s %-15s %-15s %-15s %-15s %-15s' % ('Layer', 'LD-F>2', 'LD-P>1', 'LD-Wagon>3', 'LD-engine>6', 'HD-engine>14')
for il in xrange(50):
    # print ' '.join(['%-4i'] * nissues)
    print (('%-15i ' % (il + 1)) + (' '.join(['%-15i'] * nissues)) % tuple([X[il] for X in issues]))
print '-' * 80
print ('%-15s ' % 'TOTAL') + (' '.join(['%-15i'] * nissues)) % tuple(sum(X) for X in issues)
