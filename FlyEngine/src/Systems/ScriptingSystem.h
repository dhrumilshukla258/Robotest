#pragma once


class ScriptingSystem {
public:
	ScriptingSystem();
	~ScriptingSystem();

	void Update(float);

private:
	void LoadAllScripts();

	std::function<float(float)> sentinelVelocity;
};