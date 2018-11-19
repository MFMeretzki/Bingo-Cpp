using System.Collections.Generic;

public static class Encoder
{

    //short size in bytes
    private const int SHORT_SIZE = 2;

    //int size in bytes
    private const int INT_SIZE = 4;

    //bytes used to encode the packet length
    private const int PACKET_LENGTH_SIZE = 0;

    //bytes used to encode the command
    private const int COMMAND_SIZE = 1;

    //bytes used to encode the msg header
    private const int HEADER_SIZE = PACKET_LENGTH_SIZE + COMMAND_SIZE;

    //offset where the packet length is encoded
    //private const int PACKET_LENGTH_START = 0;

    //offset where the command is encoded
    private const int COMMAND_START = 0;


    //using big endian encoding

    public static void EncodeUShort (byte[] buffer, int position, ushort value)
    {
		//buffer[position] = (byte)((value >> 8) & 0xff);
		//buffer[position + 1] = (byte)(value & 0xff);
		char[] t = value.ToString().ToCharArray();
		if (value < 10)
		{
			buffer[position] = (0 + '0');
			buffer[position + 1] = (byte)(t[0]);
		}
		else
		{
			buffer[position] = (byte)t[0];
			buffer[position + 1] = (byte)t[1];
		}
    }

    public static ushort DecodeUShort (byte[] buffer, int position)
    {
		//ushort value = (ushort)((buffer[position] << 8) | buffer[position + 1]);
		//return value;

		ushort value = (ushort)((buffer[position] - '0') * 10 + (buffer[position + 1] - '0'));
		return value;
    }

    public static void EncodeInt (byte[] buffer, int position, int value)
    {
        buffer[position] = (byte)((value >> 24) & 0xff);
        buffer[position + 1] = (byte)((value >> 16) & 0xff);
        buffer[position + 2] = (byte)((value >> 8) & 0xff);
        buffer[position + 3] = (byte)(value & 0xff);
    }

    public static int DecodeInt (byte[] buffer, int position)
    {
        int value = (buffer[position] << 24) | (buffer[position + 1] << 16)
            | (buffer[position + 2] << 8) | buffer[position + 3];

        return value;
    }

    public static void EncodeULong (byte[] buffer, int position, ulong value)
    {
        buffer[position] = (byte)((value >> 56) & 0xff);
        buffer[position + 1] = (byte)((value >> 48) & 0xff);
        buffer[position + 2] = (byte)((value >> 40) & 0xff);
        buffer[position + 3] = (byte)((value >> 32) & 0xff);
        buffer[position + 4] = (byte)((value >> 24) & 0xff);
        buffer[position + 5] = (byte)((value >> 16) & 0xff);
        buffer[position + 6] = (byte)((value >> 8) & 0xff);
        buffer[position + 7] = (byte)(value & 0xff);
    }

    public static ulong DecodeULong (byte[] buffer, int position)
    {
        ulong value = (ulong)((buffer[position] << 56) | (buffer[position + 1] << 48)
            | (buffer[position + 2] << 40) | (buffer[position + 3] << 32)
            | (buffer[position + 4] << 24) | (buffer[position + 5] << 16)
            | (buffer[position + 6] << 8) | buffer[position + 7]);

        return value;
    }

    public static void EncodeUShortVector2 (byte[] buffer, int position, UShortVector2 vec)
    {
        EncodeUShort(buffer, position, vec.x);
        EncodeUShort(buffer, position + SHORT_SIZE, vec.y);
    }

    public static void EncodeUShortVector3 (byte[] buffer, int position, UShortVector3 vec)
    {
        EncodeUShort(buffer, position, vec.x);
        EncodeUShort(buffer, position + SHORT_SIZE, vec.y);
        EncodeUShort(buffer, position + SHORT_SIZE * 2, vec.z);
    }

    public static UShortVector2 DecodeUShortVector2 (byte[] buffer, int position)
    {
        ushort x = DecodeUShort(buffer, position);
        ushort y = DecodeUShort(buffer, position + SHORT_SIZE);
        return new UShortVector2(x, y);
    }

    public static UShortVector3 DecodeUShortVector3 (byte[] buffer, int position)
    {
        ushort x = DecodeUShort(buffer, position);
        ushort y = DecodeUShort(buffer, position + SHORT_SIZE);
        ushort z = DecodeUShort(buffer, position + SHORT_SIZE * 2);
        return new UShortVector3(x, y, z);
    }


    //public static void SetPacketLength (byte[] buffer, ushort length)
    //{
    //    EncodeUShort(buffer, PACKET_LENGTH_START, length);
    //}

    //public static ushort GetPacketLength (byte[] buffer, int offset)
    //{
    //    ushort length = DecodeUShort(buffer, offset + PACKET_LENGTH_START);
    //    return length;
    //}

    public static void SetCommand (byte[] buffer, ushort command)
    {
		buffer[COMMAND_START] = (byte)(command + '0');
    }

    public static ushort GetCommand (byte[] buffer, int offset)
    {
		//ushort command = DecodeUShort(buffer, offset + COMMAND_START);
		//return command;
		ushort command = (ushort)(buffer[offset + COMMAND_START] - '0');
		return command;
    }


    public static int Encode (byte[] buffer, BaseNetData data)
    {
        int packetLength = 0;

        switch (data.command)
        {
            case ServerCommands.CLIENT_CONNECTED:
                packetLength = EncodeBaseNetData(buffer, data);
                break;
            case ServerCommands.GET_CARD:
                packetLength = EncodeUShortNetData(buffer, data);
                break;
            case ServerCommands.STARTING_NEW_GAME:
                packetLength = EncodeUShortNetData(buffer, data);
                break;
            case ServerCommands.CARDS_RESPONSE:
                packetLength = EncodeCardsResponseNetData(buffer, data);
                break;
            case ServerCommands.BALL_REVEALED:
                packetLength = EncodeUShortVector3NetData(buffer, data);
                break;
            case ServerCommands.GAME_BEGAN:
                packetLength = EncodeBaseNetData(buffer, data);
                break;
            case ServerCommands.CREDIT:
                packetLength = EncodeUShortNetData(buffer, data);
                break;
            default:
                break;
        }

		return packetLength;
    }


    public static void Decode (byte[] buffer, int offset, out BaseNetData data)
    {
        data = new BaseNetData(0);

        //ushort packetLength = GetPacketLength(buffer, offset);

        //if buffer doesnt contains all the packet return
        //if (buffer.Length - offset < packetLength) return 0;

        ushort command = GetCommand(buffer, offset);
        switch (command)
        {
            case ServerCommands.CLIENT_CONNECTED:
                DecodeBaseNetData(buffer, offset, out data);
                break;
            case ServerCommands.GET_CARD:
                DecodeUShortNetData(buffer, offset, out data);
                break;
            case ServerCommands.STARTING_NEW_GAME:
                DecodeUShortNetData(buffer, offset, out data);
                break;
            case ServerCommands.CARDS_RESPONSE:
                DecodeCardsResponseNetData(buffer, offset, out data);
                break;
            case ServerCommands.BALL_REVEALED:
                DecodeUShortVector3NetData(buffer, offset, out data);
                break;
            case ServerCommands.GAME_BEGAN:
                DecodeBaseNetData(buffer, offset, out data);
                break;
            case ServerCommands.CREDIT:
                DecodeUShortNetData(buffer, offset, out data);
                break;
            default:
                break;
        }

        //return packetLength;
    }


    #region Encoders

    private static int EncodeBaseNetData (byte[] buffer, BaseNetData data)
    {
        int packetLength = HEADER_SIZE;

        if (buffer.Length < packetLength) return 0;

        //SetPacketLength(buffer, (ushort)packetLength);
        SetCommand(buffer, data.command);

        return packetLength;
    }

    private static int EncodeBoolNetData (byte[] buffer, BaseNetData data)
    {
        BoolNetData bData = (BoolNetData)data;

        //header size + 1 byte for bool
        int packetLength = HEADER_SIZE + 1;

        if (buffer.Length < packetLength) return 0;

        int pos = HEADER_SIZE;
        if (bData.value) buffer[pos] = 1;
        else buffer[pos] = 0;

        //SetPacketLength(buffer, (ushort)packetLength);
        SetCommand(buffer, bData.command);

        return packetLength;
    }

    private static int EncodeUShortNetData (byte[] buffer, BaseNetData data)
    {
        UShortNetData usData = (UShortNetData)data;

        //header size + 2 bytes for ushort
        int packetLength = HEADER_SIZE + SHORT_SIZE;

        if (buffer.Length < packetLength) return 0;

        int pos = HEADER_SIZE;
        EncodeUShort(buffer, pos, usData.value);

        //SetPacketLength(buffer, (ushort)packetLength);
        SetCommand(buffer, usData.command);

        return packetLength;
    }

    private static int EncodeUShortBoolNetData (byte[] buffer, BaseNetData data)
    {
        UShortBoolNetData ubData = (UShortBoolNetData)data;

        //header size + short size + 1 byte for bool
        int packetLength = HEADER_SIZE + SHORT_SIZE + 1;

        if (buffer.Length < packetLength) return 0;

        int pos = HEADER_SIZE;
        EncodeUShort(buffer, pos, ubData.num);
        pos += SHORT_SIZE;
        if (ubData.value) buffer[pos] = 1;
        else buffer[pos] = 0;

        //SetPacketLength(buffer, (ushort)packetLength);
        SetCommand(buffer, ubData.command);

        return packetLength;
    }

    private static int EncodeUShortVector2NetData (byte[] buffer, BaseNetData data)
    {
        UShortVector2NetData vData = (UShortVector2NetData)data;

        int packetLength = HEADER_SIZE + SHORT_SIZE * 2;

        if (buffer.Length < packetLength) return 0;

        int pos = HEADER_SIZE;
        EncodeUShortVector2(buffer, pos, vData.vector);

        //SetPacketLength(buffer, (ushort)packetLength);
        SetCommand(buffer, vData.command);

        return packetLength;
    }

    private static int EncodeUShortVector3NetData (byte[] buffer, BaseNetData data)
    {
        UShortVector3NetData vData = (UShortVector3NetData)data;

        int packetLength = HEADER_SIZE + SHORT_SIZE * 3;

        if (buffer.Length < packetLength) return 0;

        int pos = HEADER_SIZE;
        EncodeUShortVector3(buffer, pos, vData.vector);

        //SetPacketLength(buffer, (ushort)packetLength);
        SetCommand(buffer, vData.command);

        return packetLength;
    }

    private static int EncodeCardsResponseNetData (byte[] buffer, BaseNetData data)
    {
        CardsNetData cardsData = (CardsNetData)data;

        ushort nCards = (ushort)cardsData.cards.Count;
        int packetLength = HEADER_SIZE + SHORT_SIZE * ((15 * nCards) + 1);

        if (buffer.Length < packetLength) return 0;
        int pos = HEADER_SIZE;

        EncodeUShort(buffer, pos, nCards);
		pos += SHORT_SIZE;

        foreach(Card card in cardsData.cards)
        {
            for (ushort i = 0; i < 15; ++i)
            {
                EncodeUShort(buffer, pos, card.values[i]);
				pos += SHORT_SIZE;
            }
        }

        //SetPacketLength(buffer, (ushort)packetLength);
        SetCommand(buffer, cardsData.command);

        return packetLength;
    }

    #endregion

    #region Decoders

    private static void DecodeBaseNetData (byte[] buffer, int offset, out BaseNetData data)
    {
        ushort command = GetCommand(buffer, offset);

        data = new BaseNetData(command);
    }

    private static void DecodeBoolNetData (byte[] buffer, int offset, out BaseNetData data)
    {
        int pos = offset + HEADER_SIZE;
        bool value;
        if (buffer[pos] == 0) value = false;
        else value = true;

        ushort command = GetCommand(buffer, offset);
        data = new BoolNetData(command, value);
    }

    private static void DecodeUShortNetData (byte[] buffer, int offset, out BaseNetData data)
    {
        int pos = offset + HEADER_SIZE;
        ushort value = DecodeUShort(buffer, pos);

        ushort command = GetCommand(buffer, offset);
        data = new UShortNetData(command, value);
    }

    private static void DecodeUShortBoolNetData (byte[] buffer, int offset, out BaseNetData data)
    {
        int pos = offset + HEADER_SIZE;
        ushort num = DecodeUShort(buffer, pos);
        pos += SHORT_SIZE;
        bool value;
        if (buffer[pos] == 0) value = false;
        else value = true;

        ushort command = GetCommand(buffer, offset);
        UShortBoolNetData ubData = new UShortBoolNetData(command, num, value);

        data = ubData;
    }

    private static void DecodeUShortVector2NetData (byte[] buffer, int offset, out BaseNetData data)
    {
        int pos = offset + HEADER_SIZE;
        UShortVector2 vector = DecodeUShortVector2(buffer, pos);

        ushort command = GetCommand(buffer, offset);
        data = new UShortVector2NetData(command, vector);
    }

    private static void DecodeUShortVector3NetData (byte[] buffer, int offset, out BaseNetData data)
    {
        int pos = offset + HEADER_SIZE;
        UShortVector3 vector = DecodeUShortVector3(buffer, pos);

        ushort command = GetCommand(buffer, offset);
        data = new UShortVector3NetData(command, vector);
    }

    private static void DecodeCardsResponseNetData (byte[] buffer, int offset, out BaseNetData data)
    {
        int pos = offset + HEADER_SIZE;
        ushort nCards = DecodeUShort(buffer, pos);
		pos += SHORT_SIZE;

        List<Card> cardsList = new List<Card>();
        Card card;
        for (ushort i=0; i<nCards; ++i)
        {
            ushort[] cardData = new ushort[15];
            for (ushort j=0; j<15; ++j)
            {
                cardData[j] = DecodeUShort(buffer, pos);
				pos += SHORT_SIZE;
            }
            card = new Card(cardData);
            cardsList.Add(card);
        }
        ushort command = GetCommand(buffer, offset);
        data = new CardsNetData(command, cardsList);
    }

    #endregion

}
