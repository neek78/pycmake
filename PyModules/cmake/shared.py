
# stuff shared between cm_dir.py and cm_global.py

class CMakeVersion:
    "used with cmake_minium_required and cm_policy_version command"
    def __init__(self, major, minor=None, patch=None):
        self.major = major
        self.minor = minor
        self.patch = patch

    def __str__(self):
        res = str(self.major)
        if self.minor is not None:
            res += "." + str(self.minor)
            if self.patch is not None:
                res += "." + str(self.patch)
        else:
            if self.patch is not None:
                raise ValueError("can't have patch version without minor version")

        return res

def build_version_str(version, policy_max=None):
    version_str = str(version)
    if policy_max is not None:
        version_str += ("...")
        version_str += (str(policy_max))
    return version_str
