#include <algorithm>

#include "Console.hpp"

int main()
{
	std::vector<int> tab = {1,4,3,2}; // a tab
	std::vector<int> tab2 = {5,8,7,6}; // a tab

    Console console; // create your console

	Command sort; // create a sort command
	sort.setName("sort"); // give it a name
	sort.addAlias("s"); // add an alias
	sort.setHelp("Sort the tab"); // // short desc
	sort.setManual("sort\nSort the tab\nOptions :\n-c : crescent (default)\n-d : decrescent"); // long desc for man
	sort.setFunction([&tab,&tab2](Command::OptionSplit options) // give it the tab and the options
	{
		std::function<bool(int a,int b)> f; // sorting function

		auto itr = options.find("d");
		if (itr != options.end()) // set the sorting function depending on the options passed
		{
			f = [](int a,int b)->bool{return (a > b);};
		}
		else
		{
			f = [](int a,int b)->bool{return (a < b);};
		}

        bool exec2 = false;
		itr = options.find("param");
		if (itr != options.end())
        {
            if (itr->second.size() >= 1)
            {
                if (itr->second[0] == "2")
                {
                    std::sort(tab2.begin(),tab2.end(),f); // sort
                    for (auto i : tab2) // print
                    {
                        std::cout << i << " " << std::endl;
                    }
                    exec2 = true;
                }
            }
        }

        if (!exec2)
        {
            std::sort(tab.begin(),tab.end(),f); // sort
            for (auto i : tab) // print
            {
                std::cout << i << " " << std::endl;
            }
        }
	});
	console.addCommand(sort); // add the command to the console

	console.addTask([](){std::cout << "-=-=-=-=-=-=-=-" << std::endl;}); // you can also add task that will be executed by the console after each command the user has entered

    console.run(); // run the console

    return 0;
}
