import os,sys,re,fileinput,string,shutil
from datetime import date

import numpy as np
import random
for i in range(0,10000000,50000):
    Min_=i
    Max_ = i+50000
    condorSubmit =" mv Fitted_random_numbers/D86/FitSample_D86_50k_%d_%d_*.root Fitted_random_numbers/D86/FitSample_D86_50k_%d_%d.root "%(Min_,Max_,Min_,Max_)
    print condorSubmit
    os.system(condorSubmit)
