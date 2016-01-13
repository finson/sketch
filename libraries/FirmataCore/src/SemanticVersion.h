#ifndef SemanticVersion_h
#define SemanticVersion_h

/**
 * This SemanticVersion struct defines module version identifiers
 * as discussed in the Semantic Versioning 2.0.0 Specification by
 * Tom Preston-Werner. A copy of the standard is available at semver.org.
 */

  //-------------------------------------------------------------------------

struct SemanticVersion {

  SemanticVersion(int ma,int mi, int pa=0,int pMa=0,int pMi=0, int pPa=0) :
    major(ma), minor(mi),patch(pa),pMajor(pMa),pMinor(pMi),pPatch(pPa) {}

  int major;
  int minor;
  int patch;
  int pMajor;
  int pMinor;
  int pPatch;

};

#endif
