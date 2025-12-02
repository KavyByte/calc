

#include <bits/stdc++.h>
using namespace std;

struct LFSR {
  int n;
  vector<uint8_t> state;
  vector<int> taps;

  LFSR(int n_ = 0, const vector<int> &taps_ = {}) { init(n_, taps_); }
  void init(int n_, const vector<int> &taps_) {
    n = n_;
    taps = taps_;
    state.assign(n, 0);
  }
  void seed_from_bytes(const vector<uint8_t> &seedbytes) {

    int bi = 0;
    for (int i = 0; i < n; ++i) {
      int byteIdx = bi / 8;
      int bitIdx = bi % 8;
      uint8_t bit = 0;
      if (byteIdx < (int)seedbytes.size())
        bit = (seedbytes[byteIdx] >> bitIdx) & 1;
      state[i] = bit;
      bi++;
    }
  }
  uint8_t output_bit() const { return state[n - 1] & 1; }
  uint8_t step() {

    uint8_t fb = 0;
    for (int pos : taps) {

      int idx = n - pos;
      if (idx >= 0 && idx < n)
        fb ^= state[idx];
    }

    for (int i = n - 1; i >= 1; --i)
      state[i] = state[i - 1];
    state[0] = fb;
    return output_bit();
  }
};

inline uint8_t T2map(uint8_t ct) {
  uint8_t c1 = (ct >> 1) & 1;
  uint8_t c0 = ct & 1;
  uint8_t r1 = c0;
  uint8_t r0 = c1 ^ c0;
  return (r1 << 1) | (r0);
}

vector<uint8_t> generate_keystream_bits(LFSR &l1, LFSR &l2, LFSR &l3, LFSR &l4,
                                        LFSR *l5, size_t nbits,
                                        bool include_lfsr5) {
  vector<uint8_t> ks;
  ks.reserve(nbits);

  uint8_t ct = 0, ct_prev = 0;
  for (size_t t = 0; t < nbits; ++t) {

    uint8_t x1 = l1.output_bit();
    uint8_t x2 = l2.output_bit();
    uint8_t x3 = l3.output_bit();
    uint8_t x4 = l4.output_bit();
    uint8_t x5 = 0;
    if (include_lfsr5 && l5)
      x5 = l5->output_bit();

    int sum = x1 + x2 + x3 + x4 + (include_lfsr5 ? x5 : 0);

    uint8_t high_ct = (ct >> 1) & 1;
    uint8_t st1 = (sum + high_ct) & 3;

    uint8_t T1_ct = ct & 3;
    uint8_t T2_val = T2map(ct_prev);
    uint8_t ct_next = (st1 ^ T1_ct ^ T2_val) & 3;

    uint8_t zt = x1 ^ x2 ^ x3 ^ x4 ^ (include_lfsr5 ? x5 : 0) ^ (ct & 1);
    ks.push_back(zt);

    l1.step();
    l2.step();
    l3.step();
    l4.step();
    if (include_lfsr5 && l5)
      l5->step();

    ct_prev = ct;
    ct = ct_next;
  }
  return ks;
}

#pragma pack(push, 1)
struct BMPHeader {
  uint16_t bfType;
  uint32_t bfSize;
  uint16_t bfReserved1;
  uint16_t bfReserved2;
  uint32_t bfOffBits;
};
struct BMPInfoHeader {
  uint32_t biSize;
  int32_t biWidth;
  int32_t biHeight;
  uint16_t biPlanes;
  uint16_t biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImage;
  int32_t biXPelsPerMeter;
  int32_t biYPelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;
};
#pragma pack(pop)

bool load_bmp(const string &fn, BMPHeader &hdr, BMPInfoHeader &info,
              vector<uint8_t> &pixels, vector<uint8_t> &palette) {
  ifstream f(fn, ios::binary);
  if (!f)
    return false;
  f.read((char *)&hdr, sizeof(hdr));
  if (hdr.bfType != 0x4D42)
    return false;
  f.read((char *)&info, sizeof(info));

  if (info.biBitCount == 8) {
    int ncolors = (info.biClrUsed ? info.biClrUsed : 256);
    palette.resize(ncolors * 4);
    f.read((char *)palette.data(), palette.size());
  }

  f.seekg(hdr.bfOffBits, ios::beg);

  vector<uint8_t> data((istreambuf_iterator<char>(f)),
                       istreambuf_iterator<char>());
  pixels = std::move(data);
  return true;
}

bool save_bmp(const string &fn, const BMPHeader &hdr, const BMPInfoHeader &info,
              const vector<uint8_t> &pixels, const vector<uint8_t> &palette) {
  ofstream f(fn, ios::binary);
  if (!f)
    return false;
  f.write((const char *)&hdr, sizeof(hdr));
  f.write((const char *)&info, sizeof(info));
  if (info.biBitCount == 8 && !palette.empty())
    f.write((const char *)palette.data(), palette.size());
  f.seekp(hdr.bfOffBits, ios::beg);
  f.write((const char *)pixels.data(), pixels.size());
  return true;
}

vector<uint8_t> bits_to_bytes(const vector<uint8_t> &bits) {
  size_t nbytes = (bits.size() + 7) / 8;
  vector<uint8_t> out(nbytes, 0);
  for (size_t i = 0; i < bits.size(); ++i) {
    if (bits[i])
      out[i / 8] |= (1 << (i % 8));
  }
  return out;
}

void xor_buffer_with_keystream(vector<uint8_t> &buf,
                               const vector<uint8_t> &keystream_bytes) {
  if (keystream_bytes.empty())
    return;
  size_t klen = keystream_bytes.size();
  for (size_t i = 0; i < buf.size(); ++i) {
    buf[i] ^= keystream_bytes[i % klen];
  }
}

void print_first_bits(const vector<uint8_t> &bits, size_t N = 64) {
  for (size_t i = 0; i < min(N, bits.size()); ++i)
    cout << int(bits[i]);
  cout << '\n';
}

int main(int argc, char **argv) {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <input_grayscale_bmp>\n";
    return 1;
  }
  string infile = argv[1];

  LFSR l1(25, {25, 20, 12, 8});

  LFSR l2(31, {31, 24, 16, 12});

  LFSR l3(33, {33, 28, 24, 4});

  LFSR l4(39, {39, 36, 28, 4});

  LFSR l5(40, {40, 35, 30, 3});

  vector<uint8_t> seed(16, 0x5A);

  auto make_seed_for = [&](int bits) -> vector<uint8_t> {
    int bytes = (bits + 7) / 8;
    vector<uint8_t> s(bytes);
    for (int i = 0; i < bytes; ++i)
      s[i] = seed[(i) % seed.size()] ^ (i * 31 & 0xFF);
    return s;
  };
  l1.seed_from_bytes(make_seed_for(25));
  l2.seed_from_bytes(make_seed_for(31));
  l3.seed_from_bytes(make_seed_for(33));
  l4.seed_from_bytes(make_seed_for(39));
  l5.seed_from_bytes(make_seed_for(40));

  BMPHeader hdr;
  BMPInfoHeader info;
  vector<uint8_t> pixels;
  vector<uint8_t> palette;
  if (!load_bmp(infile, hdr, info, pixels, palette)) {
    cerr << "Failed to load BMP file or unsupported BMP: " << infile << "\n";
    return 2;
  }
  cout << "Loaded BMP: " << infile << " (Width=" << info.biWidth
       << " Height=" << info.biHeight << " BitCount=" << info.biBitCount
       << ")\n";

  size_t nbits_needed = pixels.size() * 8;

  auto clone_lfsr = [&](const LFSR &orig) {
    LFSR clone;
    clone.init(orig.n, orig.taps);
    clone.state = orig.state;
    return clone;
  };
  LFSR l1_a = clone_lfsr(l1), l2_a = clone_lfsr(l2), l3_a = clone_lfsr(l3),
       l4_a = clone_lfsr(l4);
  LFSR l1_b = clone_lfsr(l1), l2_b = clone_lfsr(l2), l3_b = clone_lfsr(l3),
       l4_b = clone_lfsr(l4), l5_b = clone_lfsr(l5);

  cout << "Generating keystreams (" << nbits_needed << " bits)...\n";
  vector<uint8_t> ks_orig_bits = generate_keystream_bits(
      l1_a, l2_a, l3_a, l4_a, nullptr, nbits_needed, false);
  vector<uint8_t> ks_mod_bits = generate_keystream_bits(
      l1_b, l2_b, l3_b, l4_b, &l5_b, nbits_needed, true);

  cout << "First 64 bits - Original E0: ";
  print_first_bits(ks_orig_bits, 64);
  cout << "First 64 bits - Modified E0: ";
  print_first_bits(ks_mod_bits, 64);

  vector<uint8_t> ks_orig_bytes = bits_to_bytes(ks_orig_bits);
  vector<uint8_t> ks_mod_bytes = bits_to_bytes(ks_mod_bits);

  vector<uint8_t> pix_orig = pixels;
  vector<uint8_t> pix_e_orig = pix_orig;
  vector<uint8_t> pix_e_mod = pix_orig;

  xor_buffer_with_keystream(pix_e_orig, ks_orig_bytes);
  xor_buffer_with_keystream(pix_e_mod, ks_mod_bytes);

  save_bmp("orig_e0_encrypted.bmp", hdr, info, pix_e_orig, palette);
  save_bmp("mod_e0_encrypted.bmp", hdr, info, pix_e_mod, palette);
  cout << "Wrote orig_e0_encrypted.bmp and mod_e0_encrypted.bmp\n";

  vector<uint8_t> pix_d_orig = pix_e_orig;
  vector<uint8_t> pix_d_mod = pix_e_mod;
  xor_buffer_with_keystream(pix_d_orig, ks_orig_bytes);
  xor_buffer_with_keystream(pix_d_mod, ks_mod_bytes);

  save_bmp("orig_e0_decrypted.bmp", hdr, info, pix_d_orig, palette);
  save_bmp("mod_e0_decrypted.bmp", hdr, info, pix_d_mod, palette);
  cout << "Wrote orig_e0_decrypted.bmp and mod_e0_decrypted.bmp (should match "
          "original input if BMP format & params preserved)\n";

  return 0;
}