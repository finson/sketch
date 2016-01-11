#ifndef SemanticVersion_h
#define SemanticVersion_h

/**
 * This SemanticVersion struct defines module version identifiers
 * as discussed in the Semantic Versioning 2.0.0 Specification by
 * Tom Preston-Werner. A copy of the standard is available at semver.org.
 */

  //-------------------------------------------------------------------------

struct SemanticVersion {

  SemanticVersion() : major(0), minor(0),patch(0),rc(0) {}
  SemanticVersion(int ma,int mi, int pa, int n) : major(ma), minor(mi),patch(pa), rc(n) {}

  int major;
  int minor;
  int patch;
  int rc;

};

#endif
