#include <cassert>
#include <concepts>
#include <iostream>
#include <string_view>

// Написать коллекцию
#include <map>

struct Person
{
    unsigned id = 0;
    std::string name;
    std::string surname;
    int age = 0;
};

std::ostream& operator<<(std::ostream& out, const Person& p)
{
    return out << "{ id:" << p.id << ", name: " << p.name
        << ", surname: " << p.surname << ", age: " << p.age << "}";
}



class PersonRepository
{
public:
    // Для класса должны быть разработаны конструкторы копирования, перемещения и операторы присваивания
    PersonRepository() = default;
    PersonRepository(const PersonRepository& other) :
        m_persons(other.m_persons),
        m_nameIndex(other.m_nameIndex),
        m_surnameIndex(other.m_surnameIndex)
    {}

    PersonRepository& operator=(const PersonRepository& other)
    {
        if (this != &other)
        {
            m_persons = other.m_persons;
            m_nameIndex = other.m_nameIndex;
            m_surnameIndex = other.m_surnameIndex;
        }
        return *this;
    }

    PersonRepository(PersonRepository&& other) noexcept :
        m_persons(std::move(other.m_persons)),
        m_nameIndex(std::move(other.m_nameIndex)),
        m_surnameIndex(std::move(other.m_surnameIndex))
    {}

    PersonRepository& operator=(PersonRepository&& other) noexcept
    {
        if (this != &other)
        {
            m_persons = std::move(other.m_persons);
            m_nameIndex = std::move(other.m_nameIndex);
            m_surnameIndex = std::move(other.m_surnameIndex);
        }
        return *this;
    }

    // Добавляет человека в коллекцию
    void AddPerson(Person p)
    {
        // Метод должен обеспечивать строгую гарантию безопасности исключений
        // При попытке добавить человека с уже имеющимся id, должно выбрасываться исключение
        // std::invalid_argument
    }

    // Удаляет человека из коллекции с идентификатором, равным id
    void RemovePerson(unsigned id)
    {
        // Метод должен обеспечивать строгую гарантию безопасности исключений.
        // При отсутствии человека с таким id не должно происходить ничего.
        // Удаление должно выполняться на время, не хуже чем за O(log N)
    }

    const Person* FindPersonById(unsigned id) const
    {
        // Возвращает человека с указанным id либо nullptr, если такого человека нет.
        // Поиск должен выполняться за время, не хуже чем O(log N)
        return nullptr;
    }

    // Callback - это любой тип, который можно использовать как функцию,
    // принимающую Person по константной ссылке.
    template <std::invocable<const Person&> Callback>
    void EnumeratePeople(Callback&& callback)
    {
        // Перебирает всех людей в коллекции, вызывая для каждого из них callback
        // Порядок, в котором происходит перебор людей не принципиален
    }

    // Ищет всех людей с указанным именем и для каждого из них вызывает callback
    template <std::invocable<const Person&> Callback>
    void FindAllPeopleWithName(std::string_view name, Callback&& callback) const
    {
        // Поиск должен выполняться за время не хуже чем за O(log N)
    }

    // Ищет всех людей с указанным именем и для каждого из них вызывает callback
    template <std::invocable<const Person&> Callback>
    void FindAllPeopleWithSurname(std::string_view surname, Callback&& callback) const
    {
        // Поиск должен выполняться за время не хуже чем за O(log N)
    }

private:
    std::map<unsigned, Person> m_persons;
    std::multimap<std::string, unsigned> m_nameIndex;   //ключ - имя
    std::multimap<std::string, unsigned> m_surnameIndex;//ключ - фамилия
};

int main()
{
    PersonRepository people;
    people.AddPerson({ .id = 1, .name = "Lev", .surname = "Tolstoy", .age = 18 });
    people.AddPerson({ .id = 2, .name = "Alexey", .surname = "Tolstoy", .age = 25 });
    people.AddPerson({ .id = 3, .name = "Fedor", .surname = "Dostoevsky", .age = 40 });
    people.AddPerson({ .id = 4, .name = "Boris", .surname = "Akunin", .age = 35 });
    people.AddPerson({ .id = 5, .name = "Vladimir", .surname = "Vysotsky", .age = 35 });
    people.AddPerson({ .id = 6, .name = "Vladimir", .surname = "Nabokov", .age = 47 });

    const auto p = people.FindPersonById(5);
    assert(p != nullptr && p->surname == "Vysotsky");

    const auto p2 = people.FindPersonById(42);
    assert(p2 == nullptr);

    people.EnumeratePeople([](const Person& person) {
        std::cout << person << std::endl;
        });

    people.FindAllPeopleWithName("Vladimir", [](const Person& person) {
        std::cout << person << std::endl;
        });

    people.FindAllPeopleWithSurname("Tolstoy", [](const Person& person) {
        std::cout << person << std::endl;
        });
}