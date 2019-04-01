#include <SoftEngine.h>

class Staff : public Soft::Entity {
public:
	Staff(const Soft::Camera* camera);

	void initialize() override;
	void onUpdate(int dt) override;
	void swing();

private:
	constexpr static float SCALE = 20.0f;
	constexpr static float SIDE_DISTANCE = 20.0f;
	constexpr static float FORWARD_DISTANCE = 45.0f;
	constexpr static float VERTICAL_OFFSET = -30.0f;
	constexpr static float CAMERA_PITCH_VERTICAL_DRIFT = 40.0f;
	constexpr static float SWING_PITCH_DEGREES = 40.0f;
	constexpr static int SWING_DURATION = 250;

	static Soft::Vec3 DEFAULT_TRANSFORM_ORIGIN;
	static Soft::Vec3 SWING_TRANSFORM_ORIGIN;

	const Soft::Camera* camera = nullptr;
	Soft::Model* model = nullptr;
	float lastCameraYaw = 0.0f;
	float lastCameraPitch = 0.0f;
	Soft::Vec3 swingPitchAxis;
	Soft::Tween<float> swingTween;

	Soft::Vec3 getPitchAxis() const;
	void updatePosition();
	void updateRotation();
	void updateSwing(int dt);
};
