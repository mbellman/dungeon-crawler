#include <SoftEngine.h>

class Staff : public Soft::Entity {
public:
	Staff(const Soft::Camera* camera);

	void initialize() override;
	void onUpdate(int dt) override;
	void swing();

private:
	constexpr static float SIDE_DISTANCE = 18.0f;
	constexpr static float FORWARD_DISTANCE = 45.0f;
	constexpr static float VERTICAL_OFFSET = -30.0f;
	constexpr static float PITCH_VERTICAL_SHIFT = 40.0f;
	constexpr static float SCALE = 20.0f;

	const Soft::Camera* camera = nullptr;
	Soft::Model* model = nullptr;
	float lastCameraYaw = 0.0f;
	float lastCameraPitch = 0.0f;

	void updatePosition();
	void updateRotation();
};
