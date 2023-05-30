using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpikeTrapDemo : MonoBehaviour {

    private GameManager gameManagerScript;

    //This script goes on the SpikeTrap prefab;
    public Animator spikeTrapAnim;

    private bool trapGodMode = true;

    void Start()
    {
        GameObject gameManagerObject = GameObject.Find("GameManager");
        gameManagerScript = gameManagerObject.GetComponent<GameManager>();

        trapGodMode = true;
    }

    void Update()
    {
        if (trapGodMode != gameManagerScript.godMode)
        {
            trapGodMode = gameManagerScript.godMode; // toggle the animation flag
        }
    }

    // Use this for initialization
    void Awake()
    {
        //get the Animator component from the trap;
        spikeTrapAnim = GetComponent<Animator>();
        //start opening and closing the trap for demo purposes;
        StartCoroutine(OpenCloseTrap2());
    }


    IEnumerator OpenCloseTrap()
    {
        //play open animation;
        spikeTrapAnim.SetTrigger("open");
        //wait 2 seconds;
        yield return new WaitForSeconds(2);
        //play close animation;
        spikeTrapAnim.SetTrigger("close");
        //wait 2 seconds;
        yield return new WaitForSeconds(2);
        //Do it again;
        StartCoroutine(OpenCloseTrap());

    }

    IEnumerator OpenCloseTrap2()
    {
        while (true) // continuously loop the animation
        {
            if (!trapGodMode)
            {
                //play open animation;
                spikeTrapAnim.SetTrigger("open");
                //wait 2 seconds;
                yield return new WaitForSeconds(2);
                //play close animation;
                spikeTrapAnim.SetTrigger("close");
                //wait 2 seconds;
                yield return new WaitForSeconds(2);
            }
            else
            {
                yield return null; // yield to allow other code to run
            }
        }
    }
}