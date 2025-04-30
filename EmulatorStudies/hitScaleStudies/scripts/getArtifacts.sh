#!/bin/sh

curl --fail --output artifacts.zip --header "JOB-TOKEN: ${CI_JOB_TOKEN}" "https://gitlab.cern.ch/api/v4/projects/${CI_PROJECT_ID}/jobs/artifacts/${LCI_REF_BRANCH}/download?job=${LCI_JOB_NAME}"
unzip -d prev_artifacts artifacts.zip
true