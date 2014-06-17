final float npi = PI / 4;
final float hnpi = npi / 2;

float cut(float v) {
  if (v > npi) return npi;
  if (v < -npi) return -npi;
  return v;
}
