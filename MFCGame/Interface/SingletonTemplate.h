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

	SingletonTemplate(const SingletonTemplate&) = delete; // ���� ���� ����
	SingletonTemplate(SingletonTemplate&) = delete; // �̵� ���� ����

	SingletonTemplate& operator=(const SingletonTemplate&) = delete;  // ���� ���� ���� ����
	SingletonTemplate& operator=(SingletonTemplate&&) = delete;       // �̵� ���� ���� ����
};