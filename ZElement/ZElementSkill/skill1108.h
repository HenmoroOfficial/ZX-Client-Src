#ifndef __CPPGEN_GNET_SKILL1108
#define __CPPGEN_GNET_SKILL1108
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1108:public Skill
    {
      public:
        enum
        { SKILL_ID = 1108 };
          Skill1108 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1108Stub:public SkillStub
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
                skill->SetRatio (0.48 + 0.02 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1108Stub ():SkillStub (1108)
        {
            occupation = 26;
            name = L"Ê´ĞÄ¶¾¹Æ";
            nativename = "Ê´ĞÄ¶¾¹Æ";
            icon = "Ê´ĞÄ¶¾¹Æ.dds";
            maxlevel = 20;
            maxlearn = 12;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 4;
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
            skill_class = 4;
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
            talent[0] = 1178;
            talent[1] = 1185;
            talent[2] = 1189;
            talent_size = 3;
            action[0] = "Ê´ĞÄ¶¾¹Æ";
            action[1] = "Ê´ĞÄ¶¾¹Æ";
            action[2] = "Ê´ĞÄ¶¾¹Æ";
            action[3] = "Ê´ĞÄ¶¾¹Æ";
            action[4] = "Ê´ĞÄ¶¾¹Æ";
            action[5] = "Ê´ĞÄ¶¾¹Æ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "Ê´ĞÄ¶¾¹Æ";
            action[16] = "Ê´ĞÄ¶¾¹Æ";
            action[17] = "Ê´ĞÄ¶¾¹Æ";
            action[18] = "Ê´ĞÄ¶¾¹Æ";
            action[19] = "Ê´ĞÄ¶¾¹Æ";
            action[20] = "Ê´ĞÄ¶¾¹Æ";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = true;
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
        virtual ~ Skill1108Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1106 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (24);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.25 * (1.728 * skill->GetLevel () * skill->GetLevel () + 22.4 * skill->GetLevel () + 311.68));
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 7000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[12] = { 55, 57, 59, 61, 63, 65, 67, 69, 72, 76, 80, 84 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (22);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (20);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (0.25 * (1.728 * skill->GetLevel () * skill->GetLevel () + 22.4 * skill->GetLevel () + 311.68)),
                               48 + 2 * skill->GetLevel (),
                               20, 18, INT (1.728 * skill->GetLevel () * skill->GetLevel () + 22.4 * skill->GetLevel () + 311.68));

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
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ()) * (1 + 0.1 * skill->GetT2 ())));
            skill->GetVictim ()->SetTime (18000 - 2000 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((1.728 * skill->GetLevel () * skill->GetLevel () + 22.4 * skill->GetLevel () + 311.68) *
                                            (1 + 0.1 * skill->GetT1 ()) +
                                            0.005 * skill->GetT1 () * skill->GetLevel () * skill->GetPlayer ()->GetHp ());
            skill->GetVictim ()->SetValue ((1.728 * skill->GetLevel () * skill->GetLevel () + 22.4 * skill->GetLevel () + 311.68) * 0.1 *
                                           skill->GetT0 ());
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (6020 * skill->GetT0 ());
            skill->GetVictim ()->SetRatio (0.2);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (8000 + 100);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetValue (0.002 * skill->GetPlayer ()->GetMp () + 1);
            skill->GetVictim ()->SetAdddefence (1);
            return true;
        }
#endif
    };
}
#endif
