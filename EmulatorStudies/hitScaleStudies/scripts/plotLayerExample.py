import argparse
import tools
import ROOT
import plotlayer
import plotting


parser = argparse.ArgumentParser()
parser.add_argument('--mapping-file', default='data/FE/FeMappingV8.txt')
args = parser.parse_args()

Nlayers = 50

# Configure colour palette here
col0 = ROOT.gStyle.GetColorPalette(0)
ncols = ROOT.gStyle.GetNumberOfColors()
# Set min and max values for the z axis
min_z = 0.
max_z = 500.

# This function reads the mapping file and makes the module
# information easy to find
mapping = tools.BuildMapping(Nlayers, args.mapping_file)

latex = ROOT.TLatex()
latex.SetTextFont(42)
latex.SetTextSize(0.01)
latex.SetTextAlign(22)

for il in xrange(Nlayers):
    ilG = il + 1  # Label the layer starting from 1

    # Arguments are [name], [xlow], [xhigh], [ylow], [yhigh], [[zlow, zhigh]]
    # The last argument is optional, but can be used when you want to print a continuous
    # value as opposed to fixed discrete colours
    plot = plotlayer.MakeLayerCanvas('layer_example_%.2i' % ilG, -6, 12, -1, 17, [min_z, max_z])
    plot['cmssw'] = {}

    # Get information about the modules in this layer
    mod_dict = mapping['layers'][il]['module_dict']

    for UV, module in mod_dict.iteritems():
        # The value we want to plot
        value = UV[0] * 20 + UV[1]
        # Compute the colour
        col = int(col0 + (((value - min_z) / (max_z - min_z)) * ncols))

        # Use the module 'type' to decide whether we draw a hexagon
        # (for silicon) or trapezoid (for scintillator)
        if module['type'] <= 1:
            plot['cmssw'][UV] = plotlayer.MakeHexUV(ilG, UV)
        else:
            plot['cmssw'][UV] = plotlayer.MakeTrapIphi(UV[1], 10.5, 12)
        plot['cmssw'][UV].SetFillColor(col)
        plot['cmssw'][UV].Draw('F')
        plot['cmssw'][UV].Draw()

        # In this block we draw the module U,V label
        if module['type'] <= 1:
            x, y = plotlayer.UVtoXY(ilG, UV[0], UV[1])
        else:
            x, y = plotlayer.IphiRToXY(UV[1], 11.25)
        latex.DrawLatex(x, y, '%i, %i' % UV)


    plotting.DrawTitle(plot['pad'], 'Layer %i' % ilG, 1)
    plot['canvas'].Print('.pdf')
    plot['canvas'].Print('.png')
