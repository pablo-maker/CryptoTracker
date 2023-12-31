struct Crypto
{
  String apiName;
  char bitmap[128];
  String symbol;
  double priceUsd;
  double dayChange;
  double weekChange;
};

// ----------------------------
// Add your cryptos to the array below
//
// First argument is a valid coin id from coingecko api (find id of your coin here https://api.coingecko.com/api/v3/coins/list?include_platform=false)
// Second argument is a bitmap of a 32x32 image with cryptocurrency logo
// ----------------------------

Crypto cryptos[] = {
    {"bitcoin",
      {0x00, 0x7f, 0xfe, 0x00, 0x00, 0xff, 0xff, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xff, 0xe0, 
 0x0f, 0xff, 0xff, 0xf0, 0x1f, 0xfe, 0xff, 0xf8, 0x3f, 0xfe, 0x4f, 0xfc, 0x3f, 0xee, 0x4f, 0xfc, 
  0x7f, 0xe0, 0x4f, 0xfe, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xf0, 0xc1, 0xff, 
  0xff, 0xf0, 0xe1, 0xff, 0xff, 0xf1, 0xe1, 0xff, 0xff, 0xf0, 0xe1, 0xff, 0xff, 0xe0, 0x01, 0xff, 
  0xff, 0xe0, 0x03, 0xff, 0xff, 0xe1, 0xc1, 0xff, 0xff, 0xe3, 0xe1, 0xff, 0xff, 0x03, 0xe1, 0xff, 
  0xff, 0x00, 0x01, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xe0, 0x07, 0xff, 0x7f, 0xe4, 0xff, 0xfe, 
  0x3f, 0xe4, 0xff, 0xfc, 0x3f, 0xfc, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xf0, 
  0x07, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xc0, 0x00, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xfe, 0x00}},
    {"ethereum",
     {0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00, 
  0x00, 0x07, 0xe0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x0f, 0xf8, 0x00, 
  0x00, 0x1f, 0xf8, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x7f, 0xfe, 0x00, 
  0x00, 0x7f, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0x80, 
  0x03, 0xff, 0xff, 0xc0, 0x00, 0xff, 0xff, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x01, 0x1f, 0xf9, 0x80, 
  0x00, 0xc7, 0xe3, 0x00, 0x00, 0x71, 0x8f, 0x00, 0x00, 0x78, 0x3e, 0x00, 0x00, 0x3e, 0x7c, 0x00, 
  0x00, 0x1f, 0xf8, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x07, 0xe0, 0x00, 
  0x00, 0x03, 0xe0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {"dogecoin",
      {0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0x80, 
 0x03, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xf0, 0x1f, 0xc0, 0x0f, 0xf8, 
  0x3f, 0xc0, 0x03, 0xfc, 0x3f, 0xc0, 0x01, 0xfc, 0x7f, 0xc0, 0x00, 0xfe, 0x7f, 0xc3, 0xe0, 0xfe, 
  0x7f, 0xc3, 0xf0, 0x7e, 0x7f, 0xc3, 0xf8, 0x7e, 0x7f, 0xc1, 0xf8, 0x7e, 0x7f, 0x00, 0x38, 0x7e, 
  0x7f, 0x00, 0x38, 0x7e, 0x7f, 0x80, 0x78, 0x7e, 0x7f, 0xc3, 0xf8, 0x7e, 0x7f, 0xc3, 0xf0, 0x7e, 
  0x7f, 0xc3, 0xe0, 0xfe, 0x7f, 0xc0, 0x00, 0xfe, 0x3f, 0xc0, 0x01, 0xfc, 0x3f, 0xc0, 0x03, 0xfc, 
  0x1f, 0xc0, 0x0f, 0xf8, 0x0f, 0xff, 0xff, 0xf0, 0x07, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xc0, 
  0x01, 0xff, 0xff, 0x80, 0x00, 0xff, 0xff, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00}}      
    };
