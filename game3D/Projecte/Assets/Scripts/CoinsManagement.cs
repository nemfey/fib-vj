using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using TMPro;

public class CoinsManagement : MonoBehaviour
{
    public TextMeshProUGUI coinsText;

    void Start()
    {
        Transform coinsTransform = transform.Find("Coins");
        coinsText = coinsTransform.GetComponent<TextMeshProUGUI>();
        coinsText.text = PlayerPrefs.GetInt("CoinCount", 0).ToString();
    }

    void Update()
    {
        coinsText.text = PlayerPrefs.GetInt("CoinCount", 0).ToString();
    }
}
