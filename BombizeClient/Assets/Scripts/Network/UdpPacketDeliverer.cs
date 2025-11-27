using System;
using System.IO;
using System.Collections.Generic;
using GameNetwork.Utilities;
using UnityEngine;

public class UdpPacketDeliverer
{
    private Action<MemoryStream> _onInputGameAction;
    public event Action<MemoryStream> OnInputGameAction
    {
        add => _onInputGameAction = value;
        remove
        {
            if(_onInputGameAction == value) _onInputGameAction = null;
        }
    }

    private UdpDriver _udpDriver;
    private uint _accessKey;
    private ushort _roomNumber;

    private uint _nextPackageNumber = 1;
    private uint _expectedPackageNumber = 1;
    private LinkedList<(uint number, byte[] rawData)> _unconfirmedPackets = new();
    private MemoryStream _currentOutStream = null;
    private MemoryStream _header = new();
    private struct Settings
    {
        public uint outConfirmationNumber;
        public bool needConfirmation;
        public Settings(uint outConfirmationNumber, bool needConfirmation)
        {
            this.outConfirmationNumber = outConfirmationNumber;
            this.needConfirmation = needConfirmation;
        }
    }
    private Settings _currentSettings = new(0, false);

    public UdpPacketDeliverer(UdpDriver udpDriver, uint accessKey, ushort roomNumber)
    {
        _udpDriver = udpDriver;
        _accessKey = accessKey;
        _roomNumber = roomNumber;

        _udpDriver.OnReceive += ProcessingInputPacket;
    }

    ~UdpPacketDeliverer() // temp
    {
        _udpDriver.OnReceive -= ProcessingInputPacket;
    }

    private void ProcessingInputPacket(byte[] rawData)
    {
        var inMStream = new MemoryStream(rawData, false);

        var inNumber = Utils.ReadUInt32(inMStream);
        var hasConfirmation = Utils.ReadByte(inMStream);
        if (hasConfirmation > 0)
        {
            var confirmationNumber = Utils.ReadUInt32(inMStream);
            ConfirmPacket(confirmationNumber);
        }

        //Debug.Log($"Input packet #{inNumber} ({hasConfirmation}). Expected: {_expectedPackageNumber}");

        if (inNumber == 0)
        {
            if (inMStream.Position < inMStream.Length)
                _onInputGameAction?.Invoke(inMStream);
        }
        else if (inNumber <= _expectedPackageNumber)
        {
            _currentOutStream = new();
            _currentSettings.outConfirmationNumber = inNumber;
            _currentSettings.needConfirmation = false;

            if (inNumber == _expectedPackageNumber && inMStream.Position < inMStream.Length)
            {
                _expectedPackageNumber++;
                _onInputGameAction?.Invoke(inMStream);
            }
        }
        // else throw away the input package
    }

    public MemoryStream GetCurrentOutStream()
    {
        return _currentOutStream ??= new();
    }

    public void SetConfirmOfCurrentPackage(bool confirm = true)
    {
        _currentSettings.needConfirmation |= confirm;
    }

    public void SendCurrentData()
    {
        if(_currentOutStream != null)
        {
            WriteInHeaderAccessData();

            if(_currentSettings.needConfirmation)
                Utils.WriteToStream(_header, _nextPackageNumber);
            else
                Utils.WriteToStream(_header, (uint)0);
            
            if(_currentSettings.outConfirmationNumber > 0)
            {
                Utils.WriteToStream(_header, (byte)1);
                Utils.WriteToStream(_header, _currentSettings.outConfirmationNumber);
            }
            else
            {
                Utils.WriteToStream(_header, (byte)0);
            }

            var sendData = JoinSendData(_header.ToArray(), _currentOutStream.ToArray());
            //var sendData = JoinSendData(_header.GetBuffer(), _currentOutStream.GetBuffer());
            _currentOutStream.Dispose();

            if(_currentSettings.needConfirmation)
            {
                _unconfirmedPackets.AddLast((_nextPackageNumber, sendData));
                _nextPackageNumber++;
            }
            _udpDriver.SendData(sendData);
            ResetSelfState();
        }
    }

    private byte[] JoinSendData(byte[] header, byte[] body)
    {
        byte[] result = new byte[header.Length + body.Length];
        Buffer.BlockCopy(header, 0, result, 0, header.Length);
        Buffer.BlockCopy(body, 0, result, header.Length, body.Length);
        return result;
    }

    private void ResetSelfState()
    {
        _currentOutStream = null;
        _header.SetLength(0);
        _currentSettings.needConfirmation = false;
        _currentSettings.outConfirmationNumber = 0;
    }

    private void WriteInHeaderAccessData()
    {
        Utils.WriteToStream(_header, _roomNumber);
        Utils.WriteToStream(_header, _accessKey);
    }

    public bool RepeatSend()
    {
        if(_unconfirmedPackets.Count == 0) return false;

        foreach (var item in _unconfirmedPackets)
        {
            _udpDriver.SendData(item.rawData);
        }
        return true;
    }

    private void ConfirmPacket(uint number)
    {
        var currentNode = _unconfirmedPackets.First;
        while (currentNode != null)
        {
            var nextNode = currentNode.Next;
            if (currentNode.Value.number == number)
            {
                _unconfirmedPackets.Remove(currentNode);
                break;
            }
            currentNode = nextNode;
        }
    }
}
