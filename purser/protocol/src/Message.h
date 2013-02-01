/*
 * Message.h
 *
 *  Created on: 30 Jan 2013
 *      Author: hukka
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>


const char PROTOCOL_SIGNATURE = 'Z';

const unsigned MAX_PHONE_LEN = 32;
const unsigned MAX_TEXT_LEN = 220;

const unsigned PHONE_OFFSET = 4;
const unsigned TEXT_OFFSET = PHONE_OFFSET + MAX_PHONE_LEN;

const unsigned MESSAGE_SIZE = PHONE_OFFSET + MAX_PHONE_LEN + MAX_TEXT_LEN;


class Message
{
public:

	static Message Parse(const char* buf);

	std::string Serialize() const;

	std::string getPhone() const { return mPhone; }
	std::string getText() const { return mText; }

	void setPhone(const std::string& phone) { mPhone = phone; }
	void setText(const std::string& text) { mText = text; }

private:

	char GetCRC(const std::string& str) const;

	unsigned mPhoneLen;
	unsigned mTextLen;
	std::string mPhone;
	std::string mText;
};




#endif /* MESSAGE_H_ */
