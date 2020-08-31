class DataManager {
public:
	// Acceleration Gyro Data
	float gyro[3];
	float accel[3];
	float ypr[3];

	// Pressure Data
	float pressure[2];
	// 0번 : 정압
	// 1번 : 동압

	// GPS Data
	float latitude;
	float longitude;
	float altitude;
	float speed;


	DataManager() {
		for (int i = 0; i < 3; i++) {
			gyro[i] = 0;
			accel[i] = 0;
			ypr[i] = 0;
		}
		pressure[0] = 0;
		pressure[1] = 0;

		latitude = 0;
		longitude = 0;
		altitude = 0;
		speed = 0;
	};

	void setAccel(float* accel) {
		for (int i = 0; i < 3; i++) {
			this->accel[i] = accel[i];
		}
	}
	void setGyro(float* gyro) {
		for (int i = 0; i < 3; i++) {
			this->gyro[i] = gyro[i];
		}
	}
	void setYPR(float* ypr) {
		for (int i = 0; i < 3; i++) {
			this->ypr[i] = ypr[i];
		}
	}
	void setPressure(float* pressure) {
		// 0번 : 정압
		// 1번 : 동압
		this->pressure[0] = pressure[0];
		this->pressure[1] = pressure[1];
	}
	void setGPS(float latitude, float longitude, float atitude, float speed) {
		this->latitude = latitude;
		this->longitude = longitude;
		this->altitude = atitude;
		this->speed = speed;
	}

	float* getDataWithFloatArray() {
		integrityCode = makeIntegrityCode();

		float data[] = {
			gyro[0],
			gyro[1],
			gyro[2],
			accel[0],
			accel[1],
			accel[2],
			pressure[0],
			pressure[1],
			ypr[0],
			ypr[1],
			ypr[2],
			latitude,
			longitude,
			altitude,
			speed,
			integrityCode
		};

		float* arr = (float*)malloc(sizeof(float) * 16);

		for (int i = 0; i < 16; i++) {
			arr[i] = data[i];
		}

		return arr;
	}

	// 배열로부터 데이터를 받아오고 성공하면 true, 실패하면 false를 반환한다.
	bool setDataWithFloatArray(float* dataset) {
		int i = 0;
		for (int a = 0; a < 3; a++) {
			gyro[a] = dataset[i];
			i++;
		}
		for (int a = 0; a < 3; a++) {
			accel[a] = dataset[i];
			i++;
		}
		pressure[0] = dataset[6];
		pressure[1] = dataset[7];
		ypr[0] = dataset[8];
		ypr[1] = dataset[9];
		ypr[2] = dataset[10];
		latitude = dataset[11];
		longitude = dataset[12];
		altitude = dataset[13];
		speed = dataset[14];
		if (integrityCode != makeIntegrityCode()) {
			return false;
		}
		return true;
	}

private:
	// For Integrity
	float integrityCode;
	float makeIntegrityCode() {
		return (int)gyro[0]
			^ (int)gyro[1]
			^ (int)gyro[2]
			^ (int)accel[0]
			^ (int)accel[1]
			^ (int)accel[2]
			^ (int)ypr[0]
			^ (int)ypr[1]
			^ (int)ypr[2]
			^ (int)pressure[0]
			^ (int)pressure[1]
			^ (int)latitude
			^ (int)longitude
			^ (int)altitude
			^ (int)speed;
	}

};