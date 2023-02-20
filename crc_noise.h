#define NOISE_POLY 0x1D872B41

namespace daisysp
{

  class crc_noise
  {
    public:
      crc_noise() {}
      ~crc_noise() {}

      void Init()
      {
  	__HAL_RCC_CRC_CLK_ENABLE();

	hrng.Instance = RNG;
	__RNG_CLK_ENABLE();
  	__HAL_RNG_ENABLE(&hrng);

	HAL_RNG_GenerateRandomNumber(&hrng, &seed_);

	CRC->POL = NOISE_POLY;
	CRC->DR = seed_;
      }

      float Process(uint8_t i)
      {
	CRC->DR = i;
	rand_ = CRC->DR;

	// take the lower 16 bits and convert to a float
	frand_ = (2.0 * ((float(rand_ & 0x0000FFFF) / 65535.0f) - 0.5));
      	return frand_;
      }

      float Process()
      {
	CRC->DR = (uint32_t)frand_;
	rand_ = CRC->DR;

	// take the lower 16 bits and convert to a float
	frand_ = (2.0 * ((float(rand_ & 0x0000FFFF) / 65535.0f) - 0.5));
      	return frand_;
      }

    private:
      uint32_t rand_, seed_;
      float    frand_;
      RNG_HandleTypeDef hrng;
  };
}
