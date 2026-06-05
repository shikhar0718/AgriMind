import firebaseService from "../services/firebase.service.js";

const getLatestSensorData = async (req, res, next) => {
  try {
    const data = await firebaseService.getLatestSensorData();

    return res.status(200).json({
      success: true,
      data: {
        temperature: data.Temperature,
        humidity: data.Humidity,
        soil_ph: data.pH,
        N: data.Nitrogen,
        P: data.Phosphorus,
        K: data.Potassium,
      },
    });
  } catch (error) {
    next(error);
  }
};

export { getLatestSensorData };