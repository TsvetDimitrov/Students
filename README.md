# Students
Напишете програма, която записва в база данни следните полета за студенти:
-	Име;
-	Факултетен номер;
-	Списък от оценки.
Програмата трябва да реализира посочените по-долу функционалности:
1.	Добавяне на студент (въвеждат се име, фак. номер и брой оценки на отделни редове и списък с оценките на един ред);
2.	Добавяне на оценки на съществуващ студент (търсене по фак. номер и добавяне на оценки в списъка с оценките);
3.	Отпечатване на списъка със студенти със следната информация за всеки студент: име, факултетен номер, среден успех;
4.	Запис на списъка със студенти във файл (в дефиниран от Вас текстов или двоичен формат);
5.	Прочитане на списъка със студенти от файл в същия формат;
6.	Сортиране на студентите по факултетен номер, използвайки алгоритъма на пряката селекция (selection sort);
7.	Сортиране на студентите по среден успех в низходящ ред, използвайки алгоритъма на прякото вмъкване (insertion sort);
8.	Извеждане на досието на студент, намерен по факултетен номер. Ако списъкът със студенти е сортиран по факултетен номер, да се използва двоично търсене за намиране на студента. Съхранявайте информацията как е сортиран списъка, а не проверявайте динамично дали е сортиран. Досието на студента съдържа името му, факултетен номер, среден успех и списък от оценките му.
### Към програмата има и следните допълнителни изисквания:
1.	Организирайте въвеждането чрез меню с избор на номерирани възможности.
2.	Програмата трябва да обработва грешки и невалиден вход от потребителя по подходящ начин, като например дава възможност за повторно въвеждане, когато се въвежда число, ако потребителят е въвел невалиден текст. Не се допуска използването на изключения за обработването на невалиден вход от потребителя.
3.	Преди всяко въвеждане на потребителя трябва да се извежда пояснителен текст какво се очаква да въведе (напр. ако потребителят ще въвежда име на студент, програмата трябва да изпише „Име: “).
4.	Ако дадена функционалност се използва на повече от едно място, тя трябва да бъде отделена в собствена функция.



# Използвани технологии:
### C++, MySQL


# Как се стартира проекта?

Трябва да имате изтеглено IDE, за предпочитане Visual Studio.
Стартирате .cpp файла от директорията. 
Ctrl + F5 за пускане на програмата. 
