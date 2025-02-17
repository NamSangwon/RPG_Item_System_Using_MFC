#pragma once
template <typename T>
class SingletonTemplate
{
public:
	inline static T& GetInstance() {
		static T instance;
		return instance;
	}

protected:
	SingletonTemplate() = default;
	~SingletonTemplate() = default;

	SingletonTemplate(const SingletonTemplate&) = delete; // 복제 생성 막음
	SingletonTemplate(SingletonTemplate&) = delete; // 이동 생성 막음

	SingletonTemplate& operator=(const SingletonTemplate&) = delete;  // 복제 대입 생성 막음
	SingletonTemplate& operator=(SingletonTemplate&&) = delete;       // 이동 대입 생성 막음
};