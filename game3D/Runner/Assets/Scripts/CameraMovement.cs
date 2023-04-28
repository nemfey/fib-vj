using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMovement : MonoBehaviour
{
    public Vector3 speed;
    float playerSpeed;

    // Start is called before the first frame update
    void Start()
    {
        playerSpeed = GameObject.Find("Player").GetComponent<PlayerMovement>().speed;
        speed = new Vector3(playerSpeed / 2, 0f, playerSpeed / 2);
    }

    // Update is called once per frame
    void Update()
    {
        playerSpeed = GameObject.Find("Player").GetComponent<PlayerMovement>().speed;
        speed = new Vector3(playerSpeed / 2, 0f, playerSpeed / 2);
        transform.Translate(speed * Time.deltaTime);
    }
}
