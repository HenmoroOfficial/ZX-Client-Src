#ifndef __CPPGEN_GNET_SKILL254
#define __CPPGEN_GNET_SKILL254
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill254:public Skill
    {
      public:
        enum
        { SKILL_ID = 254 };
          Skill254 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill254Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.05 * skill->GetT3 ());
                skill->
                    SetPlus ((6 * skill->GetLevel () * skill->GetT0 () + INT (1.42 - 0.05 * skill->GetLevel ()) * (92 + 9 * skill->GetLevel ()) +
                              INT (0.05 * skill->GetLevel () + 0.55) * (0.47 * skill->GetLevel () * skill->GetLevel () + 9 * skill->GetLevel () +
                                                                        58)) * (1 + 0.08 * skill->GetT2 ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill254Stub ():SkillStub (254)
        {
            occupation = 2;
            name = L"ÎŞÍıÕ¶";
            nativename = "ÎŞÍıÕ¶";
            icon = "ÎŞÍıÕ¶.dds";
            maxlevel = 12;
            maxlearn = 9;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 0;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 624;
            talent[1] = 625;
            talent[2] = 630;
            talent[3] = 634;
            talent_size = 4;
            action[0] = "¹íÍõ×Ú_ÎŞÍıÈĞ";
            action[1] = "¹íÍõ×Ú_ÎŞÍıÈĞ";
            action[2] = "¹íÍõ×Ú_ÎŞÍıÈĞ";
            action[3] = "¹íÍõ×Ú_ÎŞÍıÈĞ";
            action[4] = "¹íÍõ×Ú_ÎŞÍıÈĞ";
            action[5] = "¹íÍõ×Ú_ÎŞÍıÈĞ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÎŞÍıÈĞ";
            action[16] = "¹íÍõ×Ú_Æï³Ë_ÎŞÍıÈĞ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÎŞÍıÈĞ";
            action[18] = "¹íÍõ×Ú_Æï³Ë_ÎŞÍıÈĞ";
            action[19] = "¹íÍõ×Ú_Æï³Ë_ÎŞÍıÈĞ";
            action[20] = "¹íÍõ×Ú_Æï³Ë_ÎŞÍıÈĞ";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill254Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 393 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.32 * skill->GetLevel () * skill->GetLevel () + 0.48 * skill->GetLevel () + 63);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 8000 - 1000 * skill->GetT1 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 55, 57, 59, 61, 63, 65, 67, 69, 71 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (0.32 * skill->GetLevel () * skill->GetLevel () + 0.48 * skill->GetLevel () + 63),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) * (92 + 9 * skill->GetLevel ()) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (0.47 * skill->GetLevel () * skill->GetLevel () +
                                                                              9 * skill->GetLevel () + 58)), INT (52 + 4.2 * skill->GetLevel ()),
                               8 + 2 * skill->GetLevel (), 0.9 + 0.6 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (52 + 4.2 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetT1 ()));
            skill->GetVictim ()->SetTime (900 + 600 * skill->GetLevel () + 100 + 3000 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.08 + 0.02 * skill->GetLevel () + 0.08 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetWeak (1);
            skill->GetVictim ()->SetProbability (1.0 * (2 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (0.12 * skill->GetT3 () * skill->GetPlayer ()->GetHp ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak1 (1);
            return true;
        }
#endif
    };
}
#endif
