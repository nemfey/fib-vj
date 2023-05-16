using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class MainMenu : MonoBehaviour
{
    public void PlayGame()
    {
        Time.timeScale = 0f;
    }

    public void QuitGame()
    {
        Debug.Log("Quit");
        Application.Quit();
    }
}
