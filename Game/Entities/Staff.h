#include <SoftEngine.h>

class Staff : public Soft::Entity {
public:
	Staff(const Soft::Camera* camera);

	void initialize() override;
	void onUpdate(int dt) override;
	void swing();

private:
	constexpr static float SIDE_DISTANCE = 40.0f;
	constexpr static float FORWARD_DISTANCE = 80.0f;
	constexpr static float SCALE = 30.0f;

	const Soft::Camera* camera = nullptr;
	Soft::Model* model = nullptr;

	void updatePosition();
};
