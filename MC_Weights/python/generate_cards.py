import ROOT

with open('MC_datasets') as f:
    files=f.readlines()

for n,fn in enumerate(files):
    output_file='output_%i' % (n+1)
    with open(output_file) as f:
        contents=f.read()
        failed='fatal' in contents
        print failed
