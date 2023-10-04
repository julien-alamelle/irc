#include "Commande.hpp"
#include <cstdlib>
#include "Server.hpp"

/* COMMANDS */

void Server::cmdPass(const Commande &cmd, User *user)
{
	if (user->isPasswordOk())
		Messages::alreadyRegistered(*user);
	if (cmd.getParams().empty())
		Messages::needMoreParams(*user, cmd);
	if (cmd.getParams()[0] == _password)
	{
		user->setPasswordOk();
		//std::cout << user->getSocket() << ": " << "PASS OK\n"; //debug
	}
	else
		Messages::incorrectPassword(*user);
}

void Server::cmdUser(const Commande &cmd, User *user)
{
	if (cmd.getParams().size() < 4)
		Messages::needMoreParams(*user, cmd);
	else if (!user->getUsername().empty())
		Messages::alreadyRegistered(*user);
	else
	{
		std::string &realname = const_cast<std::string &>(cmd.getParams()[3]);
		for (unsigned int i = 3; i < cmd.getParams().size(); i++)
			realname += " " + cmd.getParams()[i];
		if (realname[0] != ':')
		{
			// Error: realname must start with ':'
		}
		else
		{
			user->setUsername(cmd.getParams()[0]);
			user->setRealname(realname.substr(1, realname.size() - 1));
		}
	}
}

void Server::cmdNick(const Commande &cmd, User *user)
{
	if (cmd.getParams().size() != 1)
		Messages::needMoreParams(*user, cmd);
	else
	{
		try
		{
			user->setNickname(cmd.getParams()[0]);
			//std::cout << "New NICK:" << user->getNickname() << std::endl;
		}
		catch (std::exception &e)
		{
			Messages::invalidNickName(*user);
			//std::cerr << e.what() << std::endl;
			// Invalid nick
		}
	}
}

static void modeReply(Channel *chan, Commande &cmd, bool mode, char c, std::string arg)
{
	if (mode)
		cmd.getParams().at(1) = "+";
	else
		cmd.getParams().at(1) = "-";
	cmd.getParams().at(1).push_back(c);
	cmd.getParams().at(2) = arg;
	std::string response = cmd.toString();
	for (std::vector<User *>::iterator it = chan->uBegin(); it < chan->uEnd(); ++it)
	{
		(*it)->addMessage(response);
	}
}

void Server::cmdMode(const Commande &cmd, User *user)
{
	if (cmd.getParams().size() < 2)
	{
		//std::cout << "MODE: incorrect number of args " << cmd.getParams().size() << std::endl;
		return;
	}
	std::map<std::string, Channel>::iterator channelIT = this->_channels.find(cmd.getParams().at(0));
	if (channelIT == this->_channels.end())
	{
		//std::cout << "MODE: the channel does not exist " << cmd.getParams().at(0) << std::endl;
		return;
	}
	if (!channelIT->second.isOperator(user))
	{
		//std::cout << "MODE: user is not operator" << std::endl;
		return;
	}
	unsigned long nbARG = 2;
	bool	mode = false;
	std::string modes = cmd.getParams().at(1);
	if (modes.length() <= 1 || (modes[0] != '+' && modes[0] != '-'))
	{
		//std::cout << "MODE: invalide option " << modes << std::endl;
		return;
	}
	Commande reply(cmd);
	reply.getParams().resize(3);
	reply.setPrefix(":" + user->getNickname());
	if (modes[0] == '+') mode = true;
	std::string::iterator it = modes.begin();
	++it;
	while (it != modes.end())
	{
		switch (*it)
		{
		case 'i':
			channelIT->second.setInviteMode(mode);
			modeReply(&(channelIT->second), reply, mode, *it, "");
			break;

		case 't':
			channelIT->second.setTopicMode(mode);
			modeReply(&(channelIT->second), reply, mode, *it, "");
			break;

		case 'k':
			if (mode)
			{
				if (cmd.getParams().size() < nbARG + 1)
				{
					//std::cout << "MODE: no matching argument with k" << std::endl;
					return;
				}
				channelIT->second.setPassword(cmd.getParams().at(nbARG));
				modeReply(&(channelIT->second), reply, mode, *it, cmd.getParams().at(nbARG));
				++nbARG;
			}
			else
			{
				channelIT->second.setPassword("");
				modeReply(&(channelIT->second), reply, mode, *it, "");
			}
			break;

		case 'o':
		{
			if (cmd.getParams().size() < nbARG + 1)
			{
				//std::cout << "MODE: no matching argument with o" << std::endl;
				return;
			}
			User *opArg = this->findUser(cmd.getParams().at(nbARG));
			if (!opArg)
			{
				//std::cout << "MODE: the user does not exist " << cmd.getParams().at(nbARG) << std::endl;
				return;
			}
			modeReply(&(channelIT->second), reply, mode, *it, cmd.getParams().at(nbARG));
			channelIT->second.setOperator(opArg, mode);
			++nbARG;
			break;
		}

		case 'l':
			if (mode)
			{
				if (cmd.getParams().size() < nbARG + 1)
				{
					//std::cout << "MODE: no matching argument with l" << std::endl;
					return;
				}
				char *end;
				int lim = strtol(cmd.getParams().at(nbARG).c_str(), &end, 10);
				if (*end)
				{
					//std::cout << "MODE: not a valid integer " << cmd.getParams().at(nbARG) << std::endl;
					return;
				}
				channelIT->second.setUserLimit(lim);
				modeReply(&(channelIT->second), reply, mode, *it, cmd.getParams().at(nbARG));
				++nbARG;
			}
			else
			{
				channelIT->second.setUserLimit(-1);
				modeReply(&(channelIT->second), reply, mode, *it, "");
			}
			break;

		default:
			//std::cout << "MODE: invalide option " << modes << std::endl;
			return;
		}
		++it;
	}
}

void Server::cmdJoin(const Commande &cmd, User *user)
{
	if (cmd.getParams().size() < 1 || cmd.getParams().size() > 2)
	{
		//std::cout << "JOIN: incorrect number of args " << cmd.getParams().size() << std::endl;
		return;
	}
	if (cmd.getParams().at(0).at(0) != '#')
	{
		//std::cout << "JOIN: invalide channel name " << cmd.getParams().at(0) << std::endl;
		return;
	}
	std::map<std::string, Channel>::iterator channelIT = this->_channels.find(cmd.getParams().at(0));
	if (channelIT == this->_channels.end())
	{
		channelIT = this->_channels.insert(std::make_pair(cmd.getParams().at(0), Channel(user, cmd.getParams().at(0)))).first;
		if (cmd.getParams().size() == 2)
			channelIT->second.setPassword(cmd.getParams().at(1));
	}
	else
	{
		std::string key = "";
		if (cmd.getParams().size() == 2)
			key = cmd.getParams().at(1);
		if (!channelIT->second.addUser(user, key))
		{
			//std::cout <<"JOIN: fail to join the channel\n";
			return;
		}
	}
	user->joinChannel(channelIT);
	Commande ret(cmd);
	ret.setPrefix(":" + user->getNickname());
	std::string response = ret.toString();
	for (std::vector<User *>::iterator it = channelIT->second.uBegin(); it < channelIT->second.uEnd(); ++it)
	{
		(*it)->addMessage(response);
	}
	std::string topic = channelIT->second.getTopic();
	if (topic.empty())
		response = ":" + user->getNickname() + " 331 " + user->getNickname() + " " + cmd.getParams().at(0) + " :No topic is set\r\n";
	else
		response = ":" + user->getNickname() + " 332 " + user->getNickname() + " " + cmd.getParams().at(0) + " :" + topic + "\r\n";
	user->addMessage(response);
	response = ":" + user->getNickname() + " 353 " + user->getNickname() + " = " + cmd.getParams().at(0) + " :";
	for (std::vector<User *>::iterator it = channelIT->second.uBegin(); it < channelIT->second.uEnd(); ++it)
	{
		if (channelIT->second.isOperator(*it))
			response += "@";
		response += (*it)->getNickname() + " ";
	}
	response += "\r\n";
	user->addMessage(response);
	response = ":" + user->getNickname() + " 366 " + user->getNickname() + " " + cmd.getParams().at(0) + " :End of /NAMES list\r\n";
	user->addMessage(response);
}

void Server::cmdInvi(const Commande &cmd, User *user)
{
	std::string response;
	Commande reply(cmd);
	if (cmd.getParams().size() < 2)
	{
		response = ":" + user->getNickname() + " 461 " + user->getNickname() + " INVITE :Not enought parameters\r\n";
		user->addMessage(response);
		return;
	}
	User *invArg = this->findUser(cmd.getParams().at(0));
	if (!invArg)
	{
		response = ":" + user->getNickname() + " 401 " + user->getNickname() + " " + cmd.getParams().at(0) + ":No such nick/channel\r\n";
		user->addMessage(response);
		return;
	}
	std::map<std::string, Channel>::iterator channelIT = this->_channels.find(cmd.getParams().at(1));
	if (channelIT == this->_channels.end())
	{
		response = ":" + user->getNickname() + " 403 " + user->getNickname() + " " + cmd.getParams().at(1) + ":No such channel\r\n";
		user->addMessage(response);
		return;
	}
	if (!(channelIT->second.isOperator(user)))
	{
		response = ":" + user->getNickname() + " 482 " + user->getNickname() + " " + cmd.getParams().at(1) + ":You're not channel operator\r\n";
		user->addMessage(response);
		return;
	}
	int ret = channelIT->second.inviteUser(invArg);
	switch (ret)
	{
	case 341:
		response = ":" + user->getNickname() + " 341 " + user->getNickname() + " " + invArg->getNickname() + " " + cmd.getParams().at(1) + "\r\n";
		user->addMessage(response);
		reply.setPrefix(":" + user->getNickname());
		response = reply.toString();
		invArg->addMessage(response);
		break;
	case 443:
		response = ":" + user->getNickname() + " 443 " + user->getNickname() + " " + invArg->getNickname() + " " + cmd.getParams().at(0) + ":is already on channel\r\n";
		user->addMessage(response);
		break;
	default:
//		std::cerr << "error untreated case cmdInvi\n";
		break;
	}
}

void Server::cmdPMSG(const Commande &cmd, User *user)
{
	if (cmd.getParams().size() != 2)
	{
		//std::cout << "PRIVMSG: incorrect number of args " << cmd.getParams().size() << std::endl;
		return;
	}
	Commande ret(cmd);
	ret.setPrefix(":" + user->getNickname());
	User *msgArg = this->findUser(cmd.getParams().at(0));
	std::map<std::string, Channel>::iterator channelIT = this->_channels.find(cmd.getParams().at(0));
	std::string response = ret.toString();
	//std::cout << "PRIVMSG responce: " << response;
	if (msgArg)
	{
		msgArg->addMessage(response);
	}
	else if (channelIT != this->_channels.end())
	{
		std::vector<User *>::iterator it;
		for (it = channelIT->second.uBegin(); it < channelIT->second.uEnd(); ++it)
			if (user == *it)
				break;
		if (it == channelIT->second.uEnd())
		{
			//std::cout << "PRIVMSG: user is not a member of this channel\n";
			return;
		}
		for (it = channelIT->second.uBegin(); it < channelIT->second.uEnd(); ++it)
			if (user != *it)
				(*it)->addMessage(response);
	}
	else
	{
		//std::cout << "PRIVMSG: the user/channel does not exist " << cmd.getParams().at(0) << std::endl;
		return;
	}
}

void Server::cmdKick(const Commande &cmd, User *user)
{
	std::string response;
	Commande reply(cmd);
	if (cmd.getParams().size() < 2)
	{
		response = ":" + user->getNickname() + " 461 " + user->getNickname() + " KICK :Not enought parameters\r\n";
		user->addMessage(response);
		return;
	}
	std::map<std::string, Channel>::iterator channelIT = this->_channels.find(cmd.getParams().at(0));
	if (channelIT == this->_channels.end())
	{
		response = ":" + user->getNickname() + " 403 " + user->getNickname() + " " + cmd.getParams().at(0) + ":No such channel\r\n";
		user->addMessage(response);
		return;
	}
	User *kickArg = this->findUser(cmd.getParams().at(1));
	if (!kickArg)
	{
		response = ":" + user->getNickname() + " 401 " + user->getNickname() + " " + cmd.getParams().at(1) + ":No such nick/channel\r\n";
		user->addMessage(response);
		return;
	}
	if (!(channelIT->second.isOperator(user)))
	{
		response = ":" + user->getNickname() + " 482 " + user->getNickname() + " " + cmd.getParams().at(0) + ":You're not channel operator\r\n";
		user->addMessage(response);
		return;
	}

	for (std::vector<User *>::iterator it = channelIT->second.uBegin(); it < channelIT->second.uEnd(); ++it)
		if (*it == kickArg)
		{
			reply.setPrefix(":" + user->getNickname());
			response = reply.toString();
			for (std::vector<User *>::iterator it2 = channelIT->second.uBegin(); it2 < channelIT->second.uEnd(); ++it2)
				(*it2)->addMessage(response);
			this->delUserFromChannel(channelIT, kickArg);
			return ;
		}
	response = ":" + user->getNickname() + " 441 " + user->getNickname() + " " + kickArg->getNickname() + " " + cmd.getParams().at(0) + ":they aren't on that channel\r\n";
	user->addMessage(response);
}

void Server::cmdTopi(const Commande &cmd, User *user)
{
	std::string response;
	if (cmd.getParams().size() < 1)
	{
		response = ":" + user->getNickname() + " 461 " + user->getNickname() + " TOPIC :Not enought parameters\r\n";
		user->addMessage(response);
		return;
	}
	std::map<std::string, Channel>::iterator channelIT = this->_channels.find(cmd.getParams().at(0));
	if (channelIT == this->_channels.end())
	{
		response = ":" + user->getNickname() + " 403 " + user->getNickname() + " " + cmd.getParams().at(0) + ":No such channel\r\n";
		user->addMessage(response);
		return;
	}
	std::vector<User *>::iterator it;
	for (it = channelIT->second.uBegin(); it < channelIT->second.uEnd(); ++it)
		if (user == *it)
			break;
	if (it == channelIT->second.uEnd())
	{
		response = ":" + user->getNickname() + " 442 " + user->getNickname() + " " + cmd.getParams().at(0) + ":You're not on that channel\r\n";
		user->addMessage(response);
		return;
	}
	if (cmd.getParams().size() == 1)
	{
		std::string topic = channelIT->second.getTopic();
		if (topic.empty())
			response = ":" + user->getNickname() + " 331 " + user->getNickname() + " " + cmd.getParams().at(0) + " :No topic is set\r\n";
		else
			response = ":" + user->getNickname() + " 332 " + user->getNickname() + " " + cmd.getParams().at(0) + " :" + topic + "\r\n";
		user->addMessage(response);
		return;
	}
	if (channelIT->second.getTopicMode() && !channelIT->second.isOperator(user))
	{
		response = ":" + user->getNickname() + " 482 " + user->getNickname() + " " + cmd.getParams().at(0) + ":You're not channel operator\r\n";
		user->addMessage(response);
		return;
	}
	channelIT->second.setTopic(cmd.getParams().at(1).substr(1));
	Commande reply(cmd);
	reply.setPrefix(":" + user->getNickname());
	response = reply.toString();
	for (it = channelIT->second.uBegin(); it < channelIT->second.uEnd(); ++it)
		(*it)->addMessage(response);
}

void Server::cmdQuit(const Commande &cmd, User *user)
{
	try
	{
		while (true)
		{
			std::map<std::string, Channel>::iterator it = user->getChannel();
			std::string response = ":" + user->getNickname() + " PART " + it->first;
			if (cmd.getParams().size() > 0)
			{
				response += " " + cmd.getParams().at(0);
			}
			response += "\r\n";
			this->delUserFromChannel(it, user);
			if (user->getChannel() == this->_channels.end())
				break;
			for (std::vector<User *>::iterator it2 = it->second.uBegin(); it2 != it->second.uEnd(); ++it2)
				(*it2)->addMessage(response);
		}
	}
	catch (std::exception &e)
	{
		(void) e;
	}
}
