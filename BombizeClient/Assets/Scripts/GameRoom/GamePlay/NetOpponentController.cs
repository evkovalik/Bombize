using DataStructures;
using GameNetwork.NetworkData;
using UnityEngine;

public class NetOpponentController : MonoBehaviour
{
    private const float _sensitivityToDirection = 0.02f;
    private PlayerPosition _nextPlayerPosition = new();
    public PlayerPosition NextPlayerPosition {
        get => _nextPlayerPosition;
        set 
        {
            if(value.Lable > _nextPlayerPosition.Lable)
            {
                _nextPlayerPosition = value;
                _nextPosition.x = _nextPlayerPosition.Coordinate.x.Value;
                _nextPosition.y = _nextPlayerPosition.Coordinate.y.Value;

                var dv = _nextPosition - transform.position;
                var absdv = new Vector2(Mathf.Abs(dv.x), Mathf.Abs(dv.y));

                if(absdv.x > _sensitivityToDirection ||absdv.y > _sensitivityToDirection)
                {
                    if(absdv.x > absdv.y)
                    {
                        if(dv.x > 0)
                            _characterView.SetDirection(Direction.Right);
                        else if(dv.x < 0)
                            _characterView.SetDirection(Direction.Left);
                    }
                    else if(absdv.x < absdv.y)
                    {
                        if(dv.y > 0)
                            _characterView.SetDirection(Direction.Up);
                        else if(dv.y < 0)
                            _characterView.SetDirection(Direction.Down);
                    }
                }
            }
        }
    }
    private Vector3 _nextPosition = Vector3.zero;
    private CharacterView _characterView;
    private float _smooth = 30f;

    private void Awake()
    {
        _characterView = GetComponent<CharacterView>();
    }

    private void Start()
    {
        _nextPosition = transform.position;
    }

    private void FixedUpdate()
    {
        transform.position = 
            Vector3.Lerp(transform.position, _nextPosition, Time.fixedDeltaTime * _smooth
        );
    }
}